#pragma once
#include <Engine/GScript.h>
#include "GObjectBasic.h"

class GLord :
	public GObjectBasic
{
private:
	class GFSM* m_FSM;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Update() override;

	virtual void OnOverlapEnter(GCollider2D* _Other) override;

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GLord);

public:
	GLord();
	GLord(const GLord& _Origin);
	~GLord();
};