#include "pch.h"
#include "ContentUI.h"
#include "TreeUI.h"

#include <Engine/GGameObject.h>

#include <Engine/GAssetManager.h>
#include <Engine/GPathManager.h>
#include <Engine/GTaskManager.h>

#include "ImGuiManager.h"
#include "InspectorUI.h"
#include "OutlinerUI.h"

#include <Engine/GPrefab.h>

ContentUI::ContentUI()
	:m_Tree(nullptr)
	,m_NotifyHandle(nullptr)
{
}

ContentUI::~ContentUI()
{
}

void ContentUI::Init()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	AddChildUI(m_Tree);

	// 트리 옵션 설정
	m_Tree->SetSeperator(false);
	m_Tree->SetShowRoot(false);
	m_Tree->SetShowOnlyName(true);

	// 특정 항목이 선택되었을 때 호출시킬 함수(Delegate) 등록)
	m_Tree->AddSelectDelegate(this, (EUI_DELEGATE_1)&ContentUI::Selection);

	// 트리 내용 갱신
	Renew();

	// Content 폴더의 모든 에셋을 메모리로 불러온다.
	Reload();

	// 콘텐츠 폴더에 변화가 있다면 True를 반환해준다.
	m_NotifyHandle = FindFirstChangeNotification(GPathManager::GetContentPath(), true, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
}

void ContentUI::Update_UI()
{
	TreeNode* pSelected = m_Tree->GetSelectedNode();

	if (nullptr != pSelected)
		GAsset* pAsset = (GAsset*)pSelected->GetData();

	if (GAssetManager::GetInst()->IsAssetChanged())
		Renew();

	// 변경점 체크
	ObserveContent();
}

void ContentUI::Render_UI()
{
	
}

// 현재 선택된 에셋으로 Inspector창을 변경한다.
void ContentUI::Selection(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;
	string name = pNode->GetName();
	Ptr<GAsset> pAsset = (GAsset*)(pNode->GetData());

	if (nullptr == pAsset)
		return;
	InspectorUI* pInspectorUI = ImGuiManager::GetInst()->FindUI<InspectorUI>("InspectorUI");

	pInspectorUI->SetAsset(pAsset);
	
}

void ContentUI::Renew()
{
	m_Tree->Clear();

	TreeNode* pRoot = m_Tree->GetRoot();

	for (UINT i = 0; i < (int)ASSET_TYPE::END; ++i)
	{
		TreeNode* pAssetNode = m_Tree->AddItem(pRoot, ASSET_TYPE_STR[i], 0);
		pAssetNode->SetFrame(true);
		static vector <string> vecNames;
		vecNames.clear();
		GAssetManager::GetInst()->GetAssetNameList((ASSET_TYPE)i, vecNames);

		for (size_t j = 0; j < vecNames.size(); ++j)
		{
			Ptr<GAsset> pAsset = GAssetManager::GetInst()->FindAsset((ASSET_TYPE)i, wstring(vecNames[j].begin(), vecNames[j].end()));
			m_Tree->AddItem(pAssetNode, vecNames[j], (DWORD_PTR)pAsset.Get());
		}

		if (i == (int)ASSET_TYPE::PREFAB)
		{
			pAssetNode->AddDragDropDelegate("OutlinterTree", this, (EUI_DELEGATE_2)&ContentUI::AddPrefab);
		}
	}
}

void ContentUI::AddPrefab(DWORD_PTR _PayLoad, DWORD_PTR _TargetAsset)
{
	const ImGuiPayload* payload = (ImGuiPayload*)_PayLoad;
	TreeNode* pDropNode = (TreeNode*)_TargetAsset;

	GGameObject* pObject = *((GGameObject**)payload->Data);

	wchar_t buffer[255];
	swprintf_s(buffer, 255, L"Prefab\\%ls.prefab", pObject->GetName().c_str());

	Ptr<GPrefab> pPrefab = GAssetManager::GetInst()->FindAsset<GPrefab>(buffer);

	// 해당 키가 없을 때만 추가한다.
	if (pPrefab == nullptr)
	{
		pPrefab = new GPrefab();
		pPrefab->SetGameObject(pObject);
		GAssetManager::GetInst()->AddAsset(buffer, pPrefab.Get());
	}
	else
	{
		pPrefab->SetGameObject(pObject);
	}
	
	wstring SavePath = GPathManager::GetContentPath();
	SavePath += buffer;
	pPrefab->Save(SavePath);


}

/// <summary>
/// 해당 폴더에 있는 모든 파일을 불러온다.
/// </summary>
/// <param name="_FolderPath"></param>
void ContentUI::FindContentFile(const wstring& _FolderPath)
{
	wstring FolderPath = _FolderPath + L"*.*";

	WIN32_FIND_DATA FindData = {};

	// 탐색 대상 경로를 알려주면, 그곳을 탐색하는 커널오브젝트 객체가 생성되고, ID(핸들)을 리턴한다.
	HANDLE hFind = FindFirstFile(FolderPath.c_str(), &FindData);
	assert(hFind);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	while (FindNextFile(hFind, &FindData))
	{
		// 탐색한 대상이 폴더인 경우
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// 이름이 .. 인 더미폴더(상위폴더로 나가는 기능)
			if (!wcscmp(FindData.cFileName, L".."))
				continue;

			// 하위폴더 재탐색 재귀
			FindContentFile(_FolderPath + FindData.cFileName + L"\\");
		}

		// 탐색 대상이 파일인 경우
		else
		{
			wstring RelativePath = GPathManager::GetRelativePath(_FolderPath + FindData.cFileName);
			m_ContentFile.push_back(RelativePath);
		}
	}

	FindClose(hFind);
}



void ContentUI::Reload()
{
	// ====================
	// 에셋 확인
	// ====================

	// 로딩되어있는 에셋의 대응되는 파일이 존재하는 확인
	// 존재하지 않다면 삭제 (단 엔진 에셋은 제외)
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<GAsset>>& mapAsset =  GAssetManager::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			// 엔진에서 제작한 에셋은 
			if (pair.second->IsEngineAsset())
				continue;

			wstring ContentPath = GPathManager::GetContentPath();

			// 원본파일이 없어진 에셋이 메모리상에 존재하는 경우, 메모리에서도 해제시킨다.
			if (!exists(ContentPath + pair.second->GetRelativePath()))
			{
				tTask task = { TASK_TYPE::DELETE_ASSET,(DWORD_PTR)pair.second.Get(), };
				GTaskManager::GetInst()->AddTask(task);
			}
		}
	}

	// ====================
	// 로드
	// ====================
	// Content 폴더 안에 들어있는 모든 파일명 알아내기
	FindContentFile(GPathManager::GetContentPath());
	
	// 파일명으로 에셋 불러오기
	for (size_t i = 0; i < m_ContentFile.size(); ++i)
	{
		ASSET_TYPE Type = GAssetManager::GetInst()->GetExtToType(m_ContentFile[i]);

		if (ASSET_TYPE::END == Type)
			continue;

		// 확인된 타입에 맞게 파일을 메모리로 로딩
		switch (Type)
		{
		case ASSET_TYPE::MESH:
			GAssetManager::GetInst()->Load<GMesh>(m_ContentFile[i], m_ContentFile[i]);
			break;
		case ASSET_TYPE::MESHDATA:
			//CAssetMgr::GetInst()->Load<CMeshData>(m_ContentFile[i]);
			break;
		case ASSET_TYPE::TEXTURE:
			GAssetManager::GetInst()->Load<GTexture>(m_ContentFile[i], m_ContentFile[i]);
			break;
		case ASSET_TYPE::MATERIAL:
			GAssetManager::GetInst()->Load<GMaterial>(m_ContentFile[i], m_ContentFile[i]);
			break;
		case ASSET_TYPE::PREFAB:
			GAssetManager::GetInst()->Load<GPrefab>(m_ContentFile[i], m_ContentFile[i]);
			break;
		case ASSET_TYPE::SOUND:
			GAssetManager::GetInst()->Load<GSound>(m_ContentFile[i], m_ContentFile[i]);
			break;
		case ASSET_TYPE::SPRITE:
			GAssetManager::GetInst()->Load<GSprite>(m_ContentFile[i], m_ContentFile[i]);
			break;
		case ASSET_TYPE::FLIPBOOK:
			GAssetManager::GetInst()->Load<GFlipbook>(m_ContentFile[i], m_ContentFile[i]);
			break;
		case ASSET_TYPE::TILEPALETTE:
			GAssetManager::GetInst()->Load<GTilePalette>(m_ContentFile[i], m_ContentFile[i]);
			break;
		}
	}

}

void ContentUI::ObserveContent()
{
	// 즉시 확인
	DWORD dwStatus = WaitForSingleObject(m_NotifyHandle, 0);

	// 지정된 이베느가 발생했다.
	if (dwStatus == WAIT_OBJECT_0)
	{
		Reload();
		FindNextChangeNotification(m_NotifyHandle);
	}
}
