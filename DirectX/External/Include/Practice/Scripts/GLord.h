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

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GLord);

public:
	GLord();
	GLord(const GLord& _Origin);
	~GLord();
};