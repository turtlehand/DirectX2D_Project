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

public:
	void SetMoveDirection(int _Direction);

public:
	CLONE(GMinion);

public:
	GMinion();
	~GMinion();

	friend class GMinionWalkState;
	friend class GMinionAttackState;
	friend class GMinionDeadState;
};

