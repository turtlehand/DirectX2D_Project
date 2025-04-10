#include "pch.h"
#include "TreeUI.h"

#pragma region TreeNode

UINT TreeNode::g_NextID = 0;

TreeNode::TreeNode()
	: TreeUI(nullptr)
	, ID(g_NextID++)
	, Name("")
	, Data()
	, ParentNode(nullptr)
	, vecChildNode{}
	, Frame(false)
{

}

TreeNode::~TreeNode()
{
	Delete_Vec(vecChildNode);
}

void TreeNode::Render_UI()
{
	int flag = 0;
	string name = Name; 

	// 자식 노드가 없다면 화살표 표시 없음
	if (vecChildNode.empty())
	{
		flag |= ImGuiTreeNodeFlags_Leaf;
	}

	// Frame이 True라면 Frame 표시
	if (Frame)
	{
		flag |= ImGuiTreeNodeFlags_Framed;
		if (vecChildNode.empty())
			name = "   " + name;
	}

	// 현재 노드가 선택된 노드라면 선택 표시
	if (TreeUI->GetSelectedNode() == this)
	{
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	// 파일명만 보이게 한 경우, 경로문자열에서 이름만 추출해서 보이게 함
	if (TreeUI->IsShowOnlyName())
	{
		name = path(Name).stem().string();
	}

	// 출력 이름 뒤에 고유 ID 붙이기 (Key가 혹시나 같을 때 중복 방지)
	char buffer[255];
	sprintf_s(buffer, 255, "%s##%d", name.c_str(), ID);
	//name = name + "##" + to_string(ID);
	bool bSelected = false;

	if (ImGui::TreeNodeEx(buffer, flag))
	{
		// 노드가 클릭되었는지 확인
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None)) bSelected = true;

		SelfDragCheck();
		SelftDropCheck();
		DropCheck();

		for (size_t i = 0;i < vecChildNode.size();++i)
		{
			vecChildNode[i]->Render_UI();
		}
		ImGui::TreePop();
	}
	else
	{
		// 노드가 클릭되었는지 확인
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None)) bSelected = true;

		SelfDragCheck();
		SelftDropCheck();
		DropCheck();
	}

	// 이노드가 선택되었다면 UI에게 이 노드가 선택되었음을 알려줌
	if (bSelected)
		TreeUI->SetSelected(this);
}

void TreeNode::SelfDragCheck()
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		// payload 등록
		// Node를 소유한 본체(TreeUI)의 이름을 Payload 키값으로 사용한다.
		// 전달한 데이터는 Node가 들고 있는 Data이다.
		ImGui::SetDragDropPayload(TreeUI->GetName().c_str(), &Data, sizeof(DWORD_PTR));

		// 마우스를 따라다니면서 어떤 항목을 드래그한건지 정보를 보여주는 용도
		ImGui::Text(Name.c_str());

		// Tree에게 자신이 Drag 중인 노드라는 것을 알림
		TreeUI->SetDragNode(this);

		ImGui::EndDragDropSource();
	}
}

void TreeNode::SelftDropCheck()
{

	if (!TreeUI->IsSelfDragDrop())
		return;

	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI에서 드래그된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(TreeUI->GetName().c_str());

		// Tree에게 자신이 Drop 당한 노드라는 것을 알림
		if (nullptr != payload)
			TreeUI->SetDropNode(this);

		ImGui::EndDragDropTarget();
	}
}

void TreeNode::DropCheck()
{
	if (!m_DragDrop)
		return;

	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI에서 드래그된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_DUI.c_str());

		// Tree에게 자신이 Drop 당한 노드라는 것을 알림
		if (nullptr != payload)
			(m_DDInst->*m_DDFunc)((DWORD_PTR)payload, (DWORD_PTR)this);

		ImGui::EndDragDropTarget();
	}
}

#pragma endregion




TreeUI::TreeUI()
	: m_Root(nullptr)
	, m_ShowRoot(false)
	, m_ShowOnlyName(true)
	, m_SelectedNode(nullptr)
	, m_SelectedInst(nullptr)
	, m_SelectedFunc()
	, m_SelectedChange(false)
	, m_DragNode(nullptr)
	, m_DropNode(nullptr)
	, m_SDDInst(nullptr)
	, m_SDDFunc(nullptr)
	, m_SelfDragDrop(false)
{
	SetName("TreeUI");

	m_Root = new TreeNode;
	m_Root->Name = "Root";
	m_Root->TreeUI = this;
}

TreeUI::~TreeUI()
{
	if (nullptr != m_Root)
		delete m_Root;
}

void TreeUI::Clear()
{
	if (nullptr != m_Root)
	{
		Delete_Vec(m_Root->vecChildNode);
	}
}

void TreeUI::Init()
{

}

void TreeUI::Update_UI()
{

}

void TreeUI::Render_UI()
{
	// 루트노드가 없다면 중지
	if (m_Root == nullptr)
		return;
	
	if(m_ShowRoot)
		m_Root->Render_UI();
	else
	{
		for (size_t i = 0;i < m_Root->vecChildNode.size();++i)
		{
			m_Root->vecChildNode[i]->Render_UI();
		}
	}

	// 선택된 노드가 변경이 발생했다면 특정 함수 호출
	if (m_SelectedChange)
	{
		m_SelectedChange = false;
		if (m_SelectedInst && m_SelectedFunc)
		{
			(m_SelectedInst->*m_SelectedFunc)((DWORD_PTR)m_SelectedNode);
		}
	}

	if ( m_SelfDragDrop && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (m_DragNode)
		{
			if (m_DragNode != m_DropNode && (m_SDDInst && m_SDDFunc))
			{
				(m_SDDInst->*m_SDDFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
			}
		}

		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(TreeNode* _ParentNode, const string& _Name, DWORD_PTR _Data)
{
	TreeNode* pNode = new TreeNode;
	pNode->ParentNode = _ParentNode;
	pNode->Name = _Name;
	pNode->Data = _Data;
	pNode->TreeUI = _ParentNode->TreeUI;

	_ParentNode->vecChildNode.push_back(pNode);

	return pNode;
}

void TreeUI::SetSelected(TreeNode* _TreeNode)
{
	// 새로 선택한 노드와 이전에 선택한 노드가 같다면 바로 종료
	if (m_SelectedNode == _TreeNode)
		return;
	m_SelectedChange = true;
	m_SelectedNode = _TreeNode;
}




