#pragma once
#include "GObjectBasic.h"
#include <Engine/GScript.h>

class GOlderMan :
	public GObjectBasic
{
private:
	bool m_IsDead;
	float m_Timer;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Update() override;

	virtual void OnOverlapEnter(GCollider2D* _Other) override;

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GOlderMan);

public:
	GOlderMan();
	~GOlderMan();
};
