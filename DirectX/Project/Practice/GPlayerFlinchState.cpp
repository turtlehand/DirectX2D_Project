#include "pch.h"
#include "GPlayerFlinchState.h"

#include <Engine/GTimeManager.h>

#include "GPlayer.h"
#include <Engine/components.h>

void GPlayerFlinchState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
	m_PlayerRigid = GetFSM()->GameObject()->RigidBody2D();
}

void GPlayerFlinchState::Enter()
{
	m_Player->m_PlayerState = PLAYER_STATE::FLINCH;
	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::FLINCH);
	m_Timer = 0.f;
	m_Player->m_HP -= 1;
	if (m_Player->m_HP <= 0)
	{
		GGameManager::GetInst()->GameEnding(ENDING_TYPE::Mighty_Minion);
	}


	//m_Player
}

void GPlayerFlinchState::Tick()
{
	if (1.f < m_Timer)
	{
		m_Player->m_FSM->ChanageState(L"Default");
		return;
	}
	m_Timer += DT;
}

void GPlayerFlinchState::Exit()
{
}

void GPlayerFlinchState::OnOverlapEnter(GCollider2D* _Other)
{
}

void GPlayerFlinchState::OnOverlapStay(GCollider2D* _Other)
{
}

void GPlayerFlinchState::OnOverlapExit(GCollider2D* _Other)
{
}

void GPlayerFlinchState::ChangeState()
{
}

GPlayerFlinchState::GPlayerFlinchState()
{
}

GPlayerFlinchState::~GPlayerFlinchState()
{
}
