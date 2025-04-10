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

	// Ʈ�� �ɼ� ����
	m_Tree->SetSeperator(false);
	m_Tree->SetShowRoot(false);
	m_Tree->SetShowOnlyName(true);

	// Ư�� �׸��� ���õǾ��� �� ȣ���ų �Լ�(Delegate) ���)
	m_Tree->AddSelectDelegate(this, (EUI_DELEGATE_1)&ContentUI::Selection);

	// Ʈ�� ���� ����
	Renew();

	// Content ������ ��� ������ �޸𸮷� �ҷ��´�.
	Reload();

	// ������ ������ ��ȭ�� �ִٸ� True�� ��ȯ���ش�.
	m_NotifyHandle = FindFirstChangeNotification(GPathManager::GetContentPath(), true, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
}

void ContentUI::Update_UI()
{
	TreeNode* pSelected = m_Tree->GetSelectedNode();

	if (nullptr != pSelected)
		GAsset* pAsset = (GAsset*)pSelected->GetData();

	if (GAssetManager::GetInst()->IsAssetChanged())
		Renew();

	// ������ üũ
	ObserveContent();
}

void ContentUI::Render_UI()
{
	
}

// ���� ���õ� �������� Inspectorâ�� �����Ѵ�.
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

	// �ش� Ű�� ���� ���� �߰��Ѵ�.
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
/// �ش� ������ �ִ� ��� ������ �ҷ��´�.
/// </summary>
/// <param name="_FolderPath"></param>
void ContentUI::FindContentFile(const wstring& _FolderPath)
{
	wstring FolderPath = _FolderPath + L"*.*";

	WIN32_FIND_DATA FindData = {};

	// Ž�� ��� ��θ� �˷��ָ�, �װ��� Ž���ϴ� Ŀ�ο�����Ʈ ��ü�� �����ǰ�, ID(�ڵ�)�� �����Ѵ�.
	HANDLE hFind = FindFirstFile(FolderPath.c_str(), &FindData);
	assert(hFind);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	while (FindNextFile(hFind, &FindData))
	{
		// Ž���� ����� ������ ���
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// �̸��� .. �� ��������(���������� ������ ���)
			if (!wcscmp(FindData.cFileName, L".."))
				continue;

			// �������� ��Ž�� ���
			FindContentFile(_FolderPath + FindData.cFileName + L"\\");
		}

		// Ž�� ����� ������ ���
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
	// ���� Ȯ��
	// ====================

	// �ε��Ǿ��ִ� ������ �����Ǵ� ������ �����ϴ� Ȯ��
	// �������� �ʴٸ� ���� (�� ���� ������ ����)
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<GAsset>>& mapAsset =  GAssetManager::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			// �������� ������ ������ 
			if (pair.second->IsEngineAsset())
				continue;

			wstring ContentPath = GPathManager::GetContentPath();

			// ���������� ������ ������ �޸𸮻� �����ϴ� ���, �޸𸮿����� ������Ų��.
			if (!exists(ContentPath + pair.second->GetRelativePath()))
			{
				tTask task = { TASK_TYPE::DELETE_ASSET,(DWORD_PTR)pair.second.Get(), };
				GTaskManager::GetInst()->AddTask(task);
			}
		}
	}

	// ====================
	// �ε�
	// ====================
	// Content ���� �ȿ� ����ִ� ��� ���ϸ� �˾Ƴ���
	FindContentFile(GPathManager::GetContentPath());
	
	// ���ϸ����� ���� �ҷ�����
	for (size_t i = 0; i < m_ContentFile.size(); ++i)
	{
		ASSET_TYPE Type = GAssetManager::GetInst()->GetExtToType(m_ContentFile[i]);

		if (ASSET_TYPE::END == Type)
			continue;

		// Ȯ�ε� Ÿ�Կ� �°� ������ �޸𸮷� �ε�
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
	// ��� Ȯ��
	DWORD dwStatus = WaitForSingleObject(m_NotifyHandle, 0);

	// ������ �̺����� �߻��ߴ�.
	if (dwStatus == WAIT_OBJECT_0)
	{
		Reload();
		FindNextChangeNotification(m_NotifyHandle);
	}
}
