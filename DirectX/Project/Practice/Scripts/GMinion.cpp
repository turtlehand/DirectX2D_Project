#include "pch.h"
#include "GMinion.h"

#include <Engine/GAssetManager.h>
#include <Engine/components.h>
#include "GFSM.h"

#include "GMinionWalkState.h"
#include "GMinionAttackState.h"
#include "GMinionDeadState.h"

GMinion::GMinion()
	: GObjectBasic(MINION)
	, m_MinionState()
	, m_FSM(nullptr)
	, m_HorizontalMove(1)
	, m_MoveInitForce(50.f)
	, m_MoveMaxSpeed(10.f)
	, m_MinionSword(nullptr)
	, m_SwordPos()
	, m_SwordTime(1.f)
{

}

GMinion::~GMinion()
{

}

void GMinion::Init()
{
	ADD_FLOAT("InitMoveForce", &m_MoveInitForce);
	ADD_FLOAT("MaxMoveSpeed", &m_MoveMaxSpeed);

	ADD_PREFAB("SwordPrefab", &m_MinionSword);
	ADD_VECTOR3("SwrodPos", &m_SwordPos)
	ADD_FLOAT("SwrodTime", &m_SwordTime);
}

void GMinion::Begin()
{
	m_FSM = GameObject()->GetComponent<GFSM>();

	m_FSM->AddState(L"Walk", new GMinionWalkState);
	m_FSM->AddState(L"Attack", new GMinionAttackState);
	m_FSM->AddState(L"Dead", new GMinionDeadState);

	m_FSM->ChanageState(L"Walk");

	m_MinionSword = GAssetManager::GetInst()->FindAsset<GPrefab>(L"Prefab\\Sword_Minion.prefab");
}

void GMinion::Update()
{

}

void GMinion::OnOverlapEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER_ATTACK)
	{
		int Dir = Transform()->GetWorldPos().x - _Other->Transform()->GetWorldPos().x;
		Dir = Dir / abs(Dir);
		RigidBody2D()->AddForce(Vector2(Dir * m_FlinchForce.x, m_FlinchForce.y));
		if (m_MinionState != MINION_STATE::DEAD)
			m_FSM->ChanageState(L"Dead");
	}
}

void GMinion::SaveToFile(FILE* _File)
{
}

void GMinion::LoadFromFile(FILE* _File)
{
}
