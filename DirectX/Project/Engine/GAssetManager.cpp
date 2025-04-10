#include "pch.h"
#include "GAssetManager.h"

#include "assets.h"
#include "GPathManager.h"

GAssetManager::GAssetManager()
	:m_AssetChanged(false)
{
}

GAssetManager::~GAssetManager()
{

}

void GAssetManager::GetAssetNameList(ASSET_TYPE _Type, vector<string>& _vecNames)
{
	map<wstring, Ptr<GAsset>>& mapAsset = m_mapAsset[(int)_Type];
	map<wstring, Ptr<GAsset>>::iterator iter = mapAsset.begin();

	for (;iter != mapAsset.end(); ++iter)
	{
		_vecNames.push_back(string(iter->first.begin(), iter->first.end()));
	}
}

void GAssetManager::AddAsset(const wstring& _Key, Ptr<GAsset> _Asset)
{
	ASSET_TYPE Type = _Asset->GetType();
	map<wstring, Ptr<GAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	assert(iter == m_mapAsset[(UINT)Type].end());

	_Asset->m_Key = _Key;
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset));

	m_AssetChanged = true;
}

Ptr<GAsset> GAssetManager::FindAsset(ASSET_TYPE _Type, wstring _Name)
{
	map<wstring, Ptr<GAsset>>::iterator iter = m_mapAsset[(int)_Type].find(_Name);

	if (iter == m_mapAsset[(int)_Type].end())
		return nullptr;

	return iter->second.Get();
}

void GAssetManager::ReSetKey(ASSET_TYPE _Type, const wstring& _PreKey, const wstring& _PostKey)
{
	// 엔진 에셋이라면 assert
	assert(!m_mapAsset[(UINT)_Type].find(_PreKey)->second->IsEngineAsset());

	// 아래 타입만 가능
	assert(_Type == ASSET_TYPE::MATERIAL
		|| _Type == ASSET_TYPE::SPRITE
		|| _Type == ASSET_TYPE::FLIPBOOK
		|| _Type == ASSET_TYPE::PREFAB
		|| _Type == ASSET_TYPE::TILEPALETTE);

	if (_PreKey == _PostKey)
	{
		return;
	}

	path RelativePath = _PreKey;
	RelativePath = RelativePath.parent_path();
	wstring ext = GAssetManager::GetTypeToExt(_Type);
	wchar_t buffer[255];
	swprintf_s(buffer, 255, L"%ls\\%ls%ls", RelativePath.c_str(), _PostKey.c_str(), ext.c_str());

	// 먼저 바꾸고 싶은 키값이 이미 존재하는 키값이라면 assert
	assert(m_mapAsset[(UINT)_Type].find(buffer) == m_mapAsset[(UINT)_Type].end());

	// 이전 키값을 찾고 에셋이 존재하지 않는다면 assert
	map<wstring, Ptr<GAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_PreKey);
	assert(iter != m_mapAsset[(UINT)_Type].end());

	// 에셋의 키값을 변경하고 새로운 키값으로 에셋을 추가한다.
	Ptr<GAsset> BackUpAsset = iter->second;
	// 기존 키값을 삭제한다.
	iter = m_mapAsset[(UINT)_Type].erase(iter);

	BackUpAsset->m_Key = buffer;
	BackUpAsset->m_RelativePath = wstring(buffer);
	AddAsset(buffer, BackUpAsset);
}

void GAssetManager::DeleteAsset(Ptr<GAsset> _Asset)
{
	DeleteAsset(_Asset->GetType(), _Asset->GetKey());
}

void GAssetManager::DeleteAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<GAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_Key);

	if (iter == m_mapAsset[(UINT)_Type].end())
	{
		assert(nullptr);
		return;
	}

	m_mapAsset[(UINT)_Type].erase(iter);
	m_AssetChanged = true;
}


ASSET_TYPE GAssetManager::GetExtToType(const wstring& _RelativePath)
{
	// path.extension() 확장자를 추출한다.
	path Ext = path(_RelativePath).extension();

	if (Ext == L".mesh" || Ext == L".MESH")
	{
		return ASSET_TYPE::MESH;
	}
	else if (Ext == L".mdat" || Ext == L".MDAT")
	{
		return ASSET_TYPE::MESHDATA;
	}
	else if (Ext == L".png" || Ext == L".PNG"
		|| Ext == L".jpg" || Ext == L".JPG"
		|| Ext == L".jpeg" || Ext == L".JPEG"
		|| Ext == L".bmp" || Ext == L".BMP"
		|| Ext == L".tga" || Ext == L".TGA"
		|| Ext == L".dds" || Ext == L".DDS")
	{
		return ASSET_TYPE::TEXTURE;
	}
	else if (Ext == L".mtrl" || Ext == L".MTRL")
	{
		return ASSET_TYPE::MATERIAL;
	}
	else if (Ext == L".prefab" || Ext == L".PREFAB")
	{
		return ASSET_TYPE::PREFAB;
	}
	else if (Ext == L".mp3" || Ext == L".MP3"
		|| Ext == L".wav" || Ext == L".WAV"
		|| Ext == L".ogg" || Ext == L".OGG")
	{
		return ASSET_TYPE::SOUND;
	}
	else if (Ext == L".sprite" || Ext == L".SPRITE")
	{
		return ASSET_TYPE::SPRITE;
	}
	else if (Ext == L".flip" || Ext == L".FLIP")
	{
		return ASSET_TYPE::FLIPBOOK;
	}
	else if (Ext == L".tp" || Ext == L".TP")
	{
		return ASSET_TYPE::TILEPALETTE;
	}

	return ASSET_TYPE::END;
}

wstring GAssetManager::GetTypeToExt(ASSET_TYPE _Type)
{
	switch (_Type)
	{
	case ASSET_TYPE::MESH:
		return L".mesh";
		break;
	case ASSET_TYPE::MATERIAL:
		return L".mtrl";
		break;
	case ASSET_TYPE::PREFAB:
		return L".prefab";
		break;
	case ASSET_TYPE::SPRITE:
		return L".sprite";
		break;
	case ASSET_TYPE::FLIPBOOK:
		return L".flip";
		break;
	case ASSET_TYPE::TILEPALETTE:
		return L".tp";
		break;
	}
}



Ptr<GTexture> GAssetManager::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag, D3D11_USAGE _Usage)
{
	Ptr<GTexture> pTex = FindAsset<GTexture>(_Key);

	if (pTex != nullptr)
		return pTex;

	pTex = new GTexture;
	if (FAILED(pTex->Create(_Width, _Height, _Format, _Flag, _Usage)))
	{
		MessageBox(nullptr, _Key.c_str(), L"텍스쳐 생성 실패", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pTex.Get());

	return pTex;
}

Ptr<GTexture> GAssetManager::CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _tex2D)
{
	Ptr<GTexture> pTex = FindAsset<GTexture>(_Key);

	if (pTex != nullptr)
		return pTex;

	pTex = new GTexture;
	if (FAILED(pTex->Create(_tex2D)))
	{
		MessageBox(nullptr, _Key.c_str(), L"텍스쳐 생성 실패", MB_OK);
		return nullptr;
	}
	AddAsset(_Key, pTex.Get());

	return pTex;
}
