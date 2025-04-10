#pragma once
#include "GSingleton.h"

#include "GAsset.h"
#include "assets.h"
#include "GPathManager.h"

class GAssetManager :
	public GSingleton<GAssetManager>
{
	SINGLE(GAssetManager);
private:
	map<wstring, Ptr<GAsset>>   m_mapAsset[(UINT)ASSET_TYPE::END];
	bool						m_AssetChanged;

public:
	void Init();

private:
	void CreateEngineRectMesh();
	void CreateEngineTexture();
	void CreateEngineSprite();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();
	void CreateEngineMaterial();

public:
	void GetAssetNameList(ASSET_TYPE _Type, vector<string>& _vecNames);
	const map<wstring, Ptr<GAsset>>& GetAssets(ASSET_TYPE _AssetType) { return m_mapAsset[(UINT)_AssetType]; }

	void AddAsset(const wstring& _Key, Ptr<GAsset> _Asset);

	Ptr<GAsset> FindAsset(ASSET_TYPE _Type, wstring _Name);

	void ReSetKey(ASSET_TYPE _Type, const wstring& _PreKey, const wstring& _PostKey);

	bool IsAssetChanged()
	{
		bool Changed = m_AssetChanged;
		m_AssetChanged = false;
		return Changed;
	}

	template<typename T>
	Ptr<T> FindAsset(const wstring& _Key);

	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);

	void DeleteAsset(Ptr<GAsset> _Asset);
	void DeleteAsset(ASSET_TYPE _Type, const wstring& _Key);

	ASSET_TYPE GetExtToType(const wstring& _RelativePath);
	wstring GetTypeToExt(ASSET_TYPE _Type);

public:
	// Bindig : D3D11_BIND_FLAG
	Ptr<GTexture> CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	Ptr<GTexture> CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _tex2D);
};

template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (is_same_v<GMesh, T>)
		return ASSET_TYPE::MESH;
	else if constexpr (is_same_v<GTexture, T>)
		return ASSET_TYPE::TEXTURE;
	else if constexpr (is_same_v<GSprite, T>)
		return ASSET_TYPE::SPRITE;
	else if constexpr (is_same_v<GFlipbook, T>)
		return ASSET_TYPE::FLIPBOOK;
	else if constexpr (is_same_v<GMaterial, T>)
		return ASSET_TYPE::MATERIAL;
	else if constexpr (is_same_v<GGraphicShader, T>)
		return ASSET_TYPE::GRAPHIC_SHADER;
	else if constexpr (is_same_v<GComputeShader, T>)
		return ASSET_TYPE::COMPUTE_SHADER;
	else if constexpr (is_same_v<GPrefab, T>)
		return ASSET_TYPE::PREFAB;
	else if constexpr (is_same_v<GSound, T>)
		return ASSET_TYPE::SOUND;
	else if constexpr (is_same_v<GTilePalette, T>)
		return ASSET_TYPE::TILEPALETTE;
}


template<typename T>
inline Ptr<T> GAssetManager::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<GAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
		return nullptr;

	return (T*)iter->second.Get();
}

template<typename T>
inline Ptr<T> GAssetManager::Load(const wstring& _Key, const wstring& _RelativePath)
{
	if (_Key == L"Link")
	{
		int a = 0;
	}

	Ptr<T> pAsset = FindAsset<T>(_Key);

	// 컴퓨터 셰이더는 로드하지 않는다.
	if constexpr (!std::is_same_v<T, GComputeShader>)
	{
		if (nullptr != pAsset)
			return pAsset;

		wstring ContentPath = GPathManager::GetContentPath();

		pAsset = new T;
		if (FAILED(pAsset->Load(ContentPath + _RelativePath)))
		{
			MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 에러", MB_OK);
			return nullptr;
		}
		pAsset->m_RelativePath = _RelativePath;
		AddAsset(_Key, pAsset.Get());
	}

	return pAsset;
}

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// 에셋을 가리키고 있었는지 아닌지 저장
	int bAsset = !!_Asset.Get();
	fwrite(&bAsset, sizeof(int), 1, _File);

	// 에셋을 가리키고 있지 않았다면 키, 경로 기록 X
	if (!bAsset)
		return;

	wstring Key = _Asset->GetKey();
	wstring Path = _Asset->GetRelativePath();

	SaveWString(Key, _File);
	SaveWString(Path, _File);
}

template<typename T>
Ptr<T> LoadAssetRef(FILE* _File)
{
	// 에셋을 가리키고 있었는지 아닌지 불러오기
	int bAsset = 0;
	fread(&bAsset, sizeof(int), 1, _File);

	// 에셋을 가리키고 있지 않았다면 키, 경로 불러오기 X
	if (!bAsset)
		return nullptr;

	wstring Key;
	wstring Path;

	LoadWString(Key, _File);
	LoadWString(Path, _File);

	return GAssetManager::GetInst()->Load<T>(Key, Path);
}

