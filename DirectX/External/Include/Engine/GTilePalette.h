#pragma once
#include "GAsset.h"
#include "GTexture.h"

class GTilePalette :
	public GAsset
{
private:
	Ptr<GTexture>   m_Atlas;
	UINT			m_Row;
	UINT			m_Col;
	Vector2         m_LeftTop;
	Vector2         m_Slice;
	Vector2         m_Gap;

public:
	void SetAtlasTex(Ptr<GTexture> _Tex) { m_Atlas = _Tex; }
	void SetRowCol(UINT _Row, UINT _Col) { m_Row = _Row, m_Col = _Col; }
	void SetLeftTop(Vector2 _LeftTopPixel);
	void SetSlice(Vector2 _SlicePixel);
	void SetGap(Vector2 _GapPixel);

	Ptr<GTexture> GetAtlasTex() { return m_Atlas; }
	UINT GetCol() { return m_Col; }
	UINT GetRow() { return m_Row; }
	Vector2 GetLeftTop() { return m_LeftTop; }
	Vector2 GetSlice() { return m_Slice; }
	Vector2 GetGap() { return m_Gap; }

	Vector2 GetLeftTop_P() { return m_Atlas == nullptr ? Vector2() : m_LeftTop * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight()); }
	Vector2 GetSlice_P() { return  m_Atlas == nullptr ? Vector2() : m_Slice * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight()); }
	Vector2 GetGap_P() { return  m_Atlas == nullptr ? Vector2() : m_Gap * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight()); }
	
	Vector2 GetTileLeftTop(UINT _Row, UINT _Col)
	{ 
		Vector2 CurLeftTop;
		CurLeftTop.x = m_LeftTop.x + m_Slice.x * _Col + (_Col == 0 ? 0 : m_Gap.x) * _Col;
		CurLeftTop.y = m_LeftTop.y + m_Slice.y * _Row + (_Row == 0 ? 0 : m_Gap.y) * _Row;
		return CurLeftTop;
	}

	Vector2 GetTileLeftTop_P(UINT _Row, UINT _Col)
	{
		return GetTileLeftTop(_Row,_Col) * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
	}


public:
	virtual int Save(const wstring& _FilePath) override;
	virtual int Load(const wstring& _FilePath) override;

public:
	CLONE_DELETE(GTilePalette);
public:
	GTilePalette(bool _EngineAsset = false);
	~GTilePalette();
};