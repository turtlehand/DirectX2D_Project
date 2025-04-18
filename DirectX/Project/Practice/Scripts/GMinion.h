#pragma once
#include "GObjectBasic.h"
#include <Engine/GPrefab.h>

class GFSM;

enum class MINION_STATE
{
	WALK,
	ATTACK,
	DEAD,
	END
};

class GMinion :
    public GObjectBasic
{
private:
	MINION_STATE	m_MinionState;
	GFSM*			m_FSM;

	int				m_HorizontalMove;
	float           m_MoveInitForce;
	float           m_MoveMaxSpeed;

	Ptr<GPrefab>	m_MinionSword;
	Vector3			m_SwordPos;
	float			m_SwordTime;
	float			m_AttackTime;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(GCollider2D* _Other) override;

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GMinion);

public:
	GMinion();
	~GMinion();

	friend class GMinionWalkState;
	friend class GMinionAttackState;
	friend class GMinionDeadState;
};

