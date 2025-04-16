#include "pch.h"
#include "GPlayerWalkState.h"

#include "GPlayer.h"
#include <Engine/GFlipbookRender.h>
#include <Engine/GRigidBody2D.h>
#include <Engine/GTransform.h>
#include <Engine/GTimeManager.h>

GPlayerWalkState::GPlayerWalkState()
	:GScript(PLAYERWALKSTATE)
	, m_Player(nullptr)
{
}

GPlayerWalkState::~GPlayerWalkState()
{
}

void GPlayerWalkState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
	m_PlayerRigid = GetFSM()->GameObject()->RigidBody2D();
}

void GPlayerWalkState::Enter()
{
	m_Player->m_PlayerState = PLAYER_STATE::WALK;

	if (!m_Player->FlipbookRender())
		return;

	m_Player->FlipbookRender()->Play((int)m_Player->m_PlayerState);

	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
	m_Player->RigidBody2D()->AddForce(Vector2(m_Player->m_KeyInput.HorizontalMove, 0) * m_Player->m_MoveInitForce);
}

void GPlayerWalkState::Tick()
{
	if (!m_Player->m_IsGround)
	{
		m_Player->GetFSM()->ChanageState(L"Fall");
		return;
	}

	if (m_Player->m_KeyInput.Interaction)
	{
		if (m_Player->Interaction())
		{
			m_Player->GetFSM()->ChanageState(L"UseItem");
			return;
		}
	}

	if (m_Player->m_KeyInput.HorizontalMove == 0)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
		return;
	}
	
	if (m_Player->m_KeyInput.Jump)
	{
		m_Player->GetFSM()->ChanageState(L"Jump");
		return;
	}

	assert(m_PlayerRigid);

	m_PlayerRigid->AddForce(Vector2(m_Player->m_KeyInput.HorizontalMove, 0) * m_PlayerRigid->GetFriction() * 2 * DT);

	// 속력이 m_MaxMoveSpeed보다 커지면 감속
	if (m_Player->m_MoveMaxSpeed < fabs(m_PlayerRigid->GetVelocity().x))
	{
		Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();
		PlayerSpeed.x = m_Player->m_MoveMaxSpeed * m_Player->m_KeyInput.HorizontalMove;
		m_PlayerRigid->SetVelocity(PlayerSpeed);
	}
}

void GPlayerWalkState::Exit()
{
}

void GPlayerWalkState::SaveToFile(FILE* _File)
{
}

void GPlayerWalkState::LoadFromFile(FILE* _File)
{
}
