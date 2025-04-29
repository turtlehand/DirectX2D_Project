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

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GOlderMan);

public:
	GOlderMan();
	~GOlderMan();
};
