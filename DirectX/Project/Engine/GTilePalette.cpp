#include "pch.h"
#include "GTilePalette.h"

#include "GAssetManager.h"

GTilePalette::GTilePalette(bool _EngineAsset)
	:GAsset(ASSET_TYPE::TILEPALETTE)
	, m_Row(0)
	, m_Col(0)
{
}

GTilePalette::~GTilePalette()
{
}

// 픽셀 단위로 입력
void GTilePalette::SetLeftTop(Vector2 _LeftTopPixel)
{
	if (nullptr == m_Atlas)
	{
		m_Row = 0;
		m_Col = 0;
		m_LeftTop = Vector2(0.f, 0.f);
		m_Slice = Vector2(0.f, 0.f);
		m_Gap = Vector2(0.f, 0.f);
		return;
	}

	// 비율로 변경
	m_LeftTop = _LeftTopPixel / Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());;
}

void GTilePalette::SetSlice(Vector2 _SlicePixel)
{
	if (nullptr == m_Atlas)
	{
		m_Row = 0;
		m_Col = 0;
		m_LeftTop = Vector2(0.f, 0.f);
		m_Slice = Vector2(0.f, 0.f);
		m_Gap = Vector2(0.f, 0.f);
		return;
	}

	// 비율로 변경
	m_Slice = _SlicePixel / Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());;
}

void GTilePalette::SetGap(Vector2 _GapPixel)
{
	if (nullptr == m_Atlas)
	{
		m_Row = 0;
		m_Col = 0;
		m_LeftTop = Vector2(0.f, 0.f);
		m_Slice = Vector2(0.f, 0.f);
		m_Gap = Vector2(0.f, 0.f);
		return;
	}

	// 비율로 변경
	m_Gap = _GapPixel / Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());;
}

int GTilePalette::Save(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	if (pFile == nullptr)
		return E_FAIL;

	SaveAssetRef(m_Atlas, pFile);
	fwrite(&m_Row, sizeof(UINT), 1, pFile);
	fwrite(&m_Col, sizeof(UINT), 1, pFile);
	fwrite(&m_LeftTop, sizeof(Vector2), 1, pFile);
	fwrite(&m_Slice, sizeof(Vector2), 1, pFile);
	fwrite(&m_Gap, sizeof(Vector2), 1, pFile);

	fclose(pFile);
	return S_OK;
}

int GTilePalette::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	if (pFile == nullptr)
		return E_FAIL;

	m_Atlas = LoadAssetRef<GTexture>(pFile);
	fread(&m_Row, sizeof(UINT), 1, pFile);
	fread(&m_Col, sizeof(UINT), 1, pFile);
	fread(&m_LeftTop, sizeof(Vector2), 1, pFile);
	fread(&m_Slice, sizeof(Vector2), 1, pFile);
	fread(&m_Gap, sizeof(Vector2), 1, pFile);

	fclose(pFile);
	return S_OK;
}


