#include "pch.h"
#include "GSprite.h"

#include "GPathManager.h"
#include "GAssetManager.h"

GSprite::GSprite(bool _EngineAsset)
	: GAsset(ASSET_TYPE::SPRITE, _EngineAsset)
	, m_Atlas(nullptr)
	, m_LeftTop{}
	, m_Slice{}
{
}

GSprite::~GSprite()
{
}

// 픽셀 단위로 입력
void GSprite::SetLeftTop(Vector2 _LeftTopPixel)
{
	if (nullptr == m_Atlas)
	{
		m_LeftTop = Vector2(0.f, 0.f);
		m_Slice = Vector2(0.f, 0.f);
		m_Background = Vector2(0.f, 0.f);
		m_Offset = Vector2(0.f, 0.f);
		return;
	}

	// 비율로 변경
	m_LeftTop = _LeftTopPixel / Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void GSprite::SetSlice(Vector2 _SlicePixel)
{
	if (nullptr == m_Atlas)
	{
		m_LeftTop = Vector2(0.f, 0.f);
		m_Slice = Vector2(0.f, 0.f);
		m_Background = Vector2(0.f, 0.f);
		m_Offset = Vector2(0.f, 0.f);
		return;
	}
	// 비율로 변경하여 저장한다.
	m_Slice = _SlicePixel / Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());;
}

void GSprite::SetOffset(Vector2 _OffsetPixel)
{
	if (nullptr == m_Atlas)
	{
		m_LeftTop = Vector2(0.f, 0.f);
		m_Slice = Vector2(0.f, 0.f);
		m_Background = Vector2(0.f, 0.f);
		m_Offset = Vector2(0.f, 0.f);
		return;
	}

	// 비율로 변경하여 저장한다.
	m_Offset = _OffsetPixel / Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void GSprite::SetBackGround(Vector2 _BackgroundPixel)
{
	if (nullptr == m_Atlas)
	{
		m_LeftTop = Vector2(0.f, 0.f);
		m_Slice = Vector2(0.f, 0.f);
		m_Background = Vector2(0.f, 0.f);
		m_Offset = Vector2(0.f, 0.f);
		return;
	}

	// 비율로 변경하여 저장한다.
	m_Background = _BackgroundPixel / Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

int GSprite::Save(const wstring& _FilePath)
{
	// 입력된 전체경로에서 ContentPath 까지의 경로를 덜어내고, 상대경로만 잘라내서 설정해준다.
	wstring strContentPath = GPathManager::GetContentPath();
	wstring RelativePath = _FilePath.substr(strContentPath.length(), _FilePath.length());
	SetRelativePath(RelativePath);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	// 아틀라스 정보 저장
	SaveAssetRef(m_Atlas, pFile);

	// 스프라이트 정보 저장
	fwrite(&m_LeftTop, sizeof(Vector2), 1, pFile);
	fwrite(&m_Slice, sizeof(Vector2), 1, pFile);
	fwrite(&m_Offset, sizeof(Vector2), 1, pFile);
	fwrite(&m_Background, sizeof(Vector2), 1, pFile);


	fclose(pFile);

	return S_OK;
}

int GSprite::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	if (pFile == nullptr)
		return E_FAIL;

	// 아틀라스 정보 저장
	m_Atlas = LoadAssetRef<GTexture>(pFile);

	// 스프라이트 정보 저장
	fread(&m_LeftTop, sizeof(Vector2), 1, pFile);
	fread(&m_Slice, sizeof(Vector2), 1, pFile);
	fread(&m_Offset, sizeof(Vector2), 1, pFile);
	fread(&m_Background, sizeof(Vector2), 1, pFile);

	fclose(pFile);

	return S_OK;
}


