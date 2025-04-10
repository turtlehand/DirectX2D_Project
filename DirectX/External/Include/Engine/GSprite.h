#pragma once
#include "GAsset.h"
#include "GTexture.h"

class GSprite :
	public GAsset
{
private:
	Ptr<GTexture>   m_Atlas;
	Vector2         m_LeftTop;
	Vector2         m_Slice;
	Vector2         m_Offset;
	Vector2         m_Background;

public:
	void SetAtlasTex(Ptr<GTexture> _Tex) { m_Atlas = _Tex; }
	void SetLeftTop(Vector2 _LeftTopPixel);
	void SetSlice(Vector2 _SlicePixel);
	void SetOffset(Vector2 _OffsetPixel);
	void SetBackGround(Vector2 _BackgroundPixel);

	Ptr<GTexture> GetAtlasTex() { return m_Atlas; }
	Vector2 GetLeftTop() { return m_LeftTop; }
	Vector2 GetSlice() { return m_Slice; }
	Vector2 GetOffset() { return m_Offset; }
	Vector2 GetBackGround() { return m_Background; }

	Vector2 GetLeftTop_P() { return m_LeftTop * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight()); }
	Vector2 GetSlice_P() { return m_Slice * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight()); }
	Vector2 GetOffset_P() { return m_Offset * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight()); }
	Vector2 GetBackGround_P() { return m_Background * Vector2(m_Atlas->GetWidth(), m_Atlas->GetHeight()); }


public:
	virtual int Save(const wstring& _FilePath) override;
	virtual int Load(const wstring& _FilePath) override;

public:
	CLONE_DELETE(GSprite);
public:
	GSprite(bool _EngineAsset = false);
	~GSprite();
};

