#pragma once
#include <Engine/GScript.h>
#include "GObjectBasic.h"
class GPrincess :
    public GObjectBasic
{
private:
	class GFSM* m_FSM;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(GCollider2D* _Other) override;

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GPrincess);

public:
	GPrincess();
	~GPrincess();
};

