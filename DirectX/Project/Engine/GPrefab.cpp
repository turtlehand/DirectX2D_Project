#include "pch.h"
#include "GPrefab.h"

#include "GGameObject.h"

GAMEOBJECT_SAVE  GPrefab::g_Save = nullptr;
GAMEOBJECT_LOAD  GPrefab::g_Load = nullptr;

GPrefab::GPrefab(bool _EngineAsset )
	: GAsset(ASSET_TYPE::PREFAB, _EngineAsset)
	, m_PrefabObject(nullptr)
{
}

GPrefab::~GPrefab()
{
	if (nullptr != m_PrefabObject)
		delete m_PrefabObject;
}

void GPrefab::SetGameObject(GGameObject* _Object)
{
	if (nullptr != m_PrefabObject)
		delete m_PrefabObject;

	m_PrefabObject = _Object->Clone();

	// 프리팹으로 전환하려는 오브젝트가 레벨안에 존재하고 있다면 레이어에서 제거
	if (-1 != _Object->GetLayer())
	{
		//m_PrefabObject->DisconnectWithLayerAsParent();
		//m_PrefabObject->m_Layer = -1;
	}
}

GGameObject* GPrefab::Instantiate()
{
	return m_PrefabObject->Clone();
}

int GPrefab::Save(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	g_Save(m_PrefabObject, pFile);

	fclose(pFile);

	return S_OK;
}

int GPrefab::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	if (pFile == nullptr)
		return E_FAIL;

	m_PrefabObject = g_Load(pFile);

	fclose(pFile);
	return 0;
}


