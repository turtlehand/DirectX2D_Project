#include "pch.h"
#include "GMinionAttackState.h"

#include <Engine/GTimeManager.h>

#include <Engine/components.h>
#include "GMinion.h"

GMinionAttackState::GMinionAttackState()
	: m_Minion(nullptr)

	,m_Sword(nullptr)
	,m_Timer(0.f)
{
}

GMinionAttackState::~GMinionAttackState()
{
}

void GMinionAttackState::Awake()
{
	m_Minion = GetFSM()->GameObject()->GetComponent<GMinion>();
}

void GMinionAttackState::Enter()
{
	m_Minion->m_MinionState = MINION_STATE::ATTACK;

	m_Minion->FlipbookRender()->Play((int)MINION_STATE::ATTACK);

	m_Minion->RigidBody2D()->SetVelocityX(0);

	// 검 생성
	m_Sword = m_Minion->m_MinionSword->Instantiate();
	m_Timer = 0;

	SpawnGameObject(m_Sword);
	m_Minion->GameObject()->SetChild(m_Sword);

}

void GMinionAttackState::Tick()
{
	// 시간을 완료시 삭제
	if (m_Timer > m_Minion->m_SwordTime)
	{
		m_Minion->m_FSM->ChanageState(L"Walk");
	}
	m_Timer += DT;
}

void GMinionAttackState::Exit()
{
	m_Sword->Destroy();
}


