#pragma once
#include "GObjectBasic.h"
#include <Engine/GScript.h>

class GOlderMan :
	public GObjectBasic
{
private:
	bool m_IsDead;		// 노인이 죽었는지 
	float m_Timer;		// 노인 쓰러지는 효과용 타이머

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
