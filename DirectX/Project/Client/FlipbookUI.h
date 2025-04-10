#pragma once
#include "AssetUI.h"

class GFlipbook;

class FlipbookUI :
	public AssetUI
{
private:
	Ptr<GFlipbook>          m_Flip;
	int                     m_SelectIdx;			// Drop ������ Index

	int                     m_AnimationIdx;			// ���� ������� �ִϸ��̼�	
	float                   m_Time;					// �ִϸ��̼� ���� ���� �ð�
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

