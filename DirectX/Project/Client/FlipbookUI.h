#pragma once
#include "AssetUI.h"

class GFlipbook;

class FlipbookUI :
	public AssetUI
{
private:
	Ptr<GFlipbook>          m_Flip;
	int                     m_SelectIdx;			// Drop 삽입할 Index

	int                     m_AnimationIdx;			// 현재 재생중인 애니메이션	
	float                   m_Time;					// 애니메이션 진행 중인 시간
	bool                    m_Finish;				

public:
	virtual void Update_UI() override;
	virtual void Render_UI() override;

	virtual void Activate() override;

private:
	void RePlay();


	void AnimationList();
	void Sprite(int _Idx, const string& _SpriteKey);
	void Info(int _Idx);

	void DropCheckSprite(int _Idx);
	void ListButtonSprite(int _Idx);
	void Image(int _Idx);


	void SelectSprite(DWORD_PTR _UI);


public:
	FlipbookUI();
	~FlipbookUI();
};

