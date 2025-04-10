#include "pch.h"
#include "OutlinerUI.h"
#include "TreeUI.h"
#include "InspectorUI.h"

#include <Engine/GLevelManager.h>
#include <Engine/GTaskManager.h>
#include "ImGuiManager.h"
#include "ListUI.h"

#include <Engine/GAssetManager.h>

#include <Engine/GLevel.h>
#include <Engine/GLayer.h>
#include <Engine/GGameObject.h>
#include <Engine/GPrefab.h>

#include "GLevelSaveLoad.h"

#include <Engine/GPrefab.h>
#include <Engine/components.h>


OutlinerUI::OutlinerUI()
	:m_Tree(nullptr)
{
}

OutlinerUI::~OutlinerUI()
{
}

void OutlinerUI::Init()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinterTree");
	AddChildUI(m_Tree);

	m_Tree->SetSeperator(false);
	m_Tree->SetShowRoot(false);
	m_Tree->SetShowOnlyName(false);

	// 특정 항목이 선택되었을 때 호출시킬 함수(Delegate) 등록
	m_Tree->AddSelectDelegate(this, (EUI_DELEGATE_1)&OutlinerUI::Selection);

	// 트리 내에서 자체적으로 Drag Drop 이 발생할 경우 호출시킬 멤버함수(Delegate) 등록
	m_Tree->AddSelfDragDropDelegate(this, (EUI_DELEGATE_2)&OutlinerUI::DragDrop);
	
}

void OutlinerUI::Update_UI()
{
	// 현재 레벨에 변경점이 발생한 프레임에는 트리를 다시 구성한다.
	if (GTaskManager::GetInst()->IsLevelChanged())
	{
		Renew();
	}
}

void OutlinerUI::Render_UI()
{
	TreeNode* SelectedNode = nullptr;
	
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
	{
		ImGui::OpenPopup("OutlinerPopUp");
		SelectedNode = m_Tree->GetSelectedNode();
	}

	if (ImGui::BeginPopup("OutlinerPopUp"))
	{

		if (ImGui::Button("GameObject"))
		{
			ImGuiManager::GetInst()->CreateGameObject();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("...##Prefab"))
		{
			ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
			assert(pList);

			pList->SetActive(true);

			vector<string> Items;
			GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::PREFAB, Items);
			pList->SetItems(Items);

			pList->AddDelegate(this, (EUI_DELEGATE_1)&OutlinerUI::SelectPrefab);
		}
			
		ImGui::EndPopup();
	}
}

void OutlinerUI::Renew()
{
	// 루트를 제외한 모든 노드를 삭제
	m_Tree->Clear();

	// 루트노드 가져옴
	TreeNode* pRoot = m_Tree->GetRoot();

	// 게임 오브젝트르 가져옴
	GLevel* pLevel = GLevelManager::GetInst()->GetCurrentLevel();
	assert(pLevel);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		GLayer* pLayer = pLevel->GetLayer(i);

		const vector<GGameObject*>& vecObj = pLayer->GetParentObjects();
		
		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			AddGameObject(pRoot, vecObj[j]);
		}
	}

	
}

void OutlinerUI::AddGameObject(TreeNode* _Parent, GGameObject* _Objcet)
{
	TreeNode* CurNode = m_Tree->AddItem(_Parent
		, string(_Objcet->GetName().begin(), _Objcet->GetName().end())
		, (DWORD_PTR)_Objcet);

	//CurNode->AddDragDropDelegate("ContentTree", this, (EUI_DELEGATE_2)&OutlinerUI::AddPrefab);

	for (size_t i = 0; i < _Objcet->GetChild().size(); ++i)
	{
		AddGameObject(CurNode, _Objcet->GetChild()[i]);
	}
}

/// <summary>
/// 우선 사용 X
/// </summary>
/// <param name="_PayLoad"></param>
/// <param name="_TargetObject"></param>
void OutlinerUI::AddPrefab(DWORD_PTR _PayLoad, DWORD_PTR _TargetObject)
{
	const ImGuiPayload* payload = (ImGuiPayload*)_PayLoad;
	TreeNode* pDropNode = (TreeNode*)_TargetObject;

	GPrefab* pPrefab = *((GPrefab**)payload->Data);

	GGameObject* pPrefabObject = pPrefab->Instantiate();

	GGameObject* pParent = nullptr;
	if (nullptr != pDropNode)
		pParent = (GGameObject*)pDropNode->GetData();

	if (!IsValid(pParent))
		return;

	tTask task = {};
	task.Type = TASK_TYPE::ADD_CHILD;
	task.Param0 = (DWORD_PTR)pParent;
	task.Param1 = (DWORD_PTR)pPrefabObject;
	GTaskManager::GetInst()->AddTask(task);
}


void OutlinerUI::Selection(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	string Name = pNode->GetName();
	GGameObject* pObject = (GGameObject*)pNode->GetData();

	if (nullptr == pObject)
		return;

	InspectorUI* pInspector = ImGuiManager::GetInst()->FindUI<InspectorUI>("InspectorUI");
	pInspector->SetTargetObject(pObject);
}

void OutlinerUI::SelectPrefab(DWORD_PTR _Node)
{
	ListUI* pList = (ListUI*)_Node;
	Ptr<GPrefab> pPrefab = GAssetManager::GetInst()->FindAsset<GPrefab>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));

	GGameObject* pObject = pPrefab->Instantiate();

	SpawnGameObject(pObject);
}

void OutlinerUI::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _TargetNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_TargetNode;

	GGameObject* pChild = (GGameObject*)pDragNode->GetData();
	GGameObject* pParent = nullptr;
	if(nullptr != pDropNode)
		pParent = (GGameObject*)pDropNode->GetData();

	// 부모가 될 오브젝트가 있을 때 이미 조상이거나 이미 부모이면 안됨
	if (pParent && (pParent->IsAncestor(pChild) || pParent == pChild->GetParent()))
		return;

	tTask task = {};
	task.Type = TASK_TYPE::ADD_CHILD;
	task.Param0 = (DWORD_PTR)pParent;
	task.Param1 = (DWORD_PTR)pChild;
	GTaskManager::GetInst()->AddTask(task);

}

