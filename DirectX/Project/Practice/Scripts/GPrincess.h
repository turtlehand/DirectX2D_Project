#pragma once
#include <Engine/GScript.h>
#include "GObjectBasic.h"
class GPrincess :
    public GObjectBasic
{
private:
	const PLAYER_ITEM m_ItemType;
	bool m_IsDead;
	float m_Timer;

public:
	PLAYER_ITEM const GetItemType() { return m_ItemType; }

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Update() override;

	virtual void OnOverlapEnter(GCollider2D* _Other) override;

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GPrincess);

public:
	GPrincess();
	~GPrincess();
};