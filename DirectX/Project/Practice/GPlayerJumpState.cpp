#include "pch.h"
#include "GPlayerJumpState.h"

#include "GPlayer.h"
#include <Engine/GFlipbookRender.h>
#include <Engine/GRigidBody2D.h>
#include <Engine/GTransform.h>
#include <Engine/GTimeManager.h>

GPlayerJumpState::GPlayerJumpState()
	: GScript(PLAYERJUMPSTATE)
{
}

GPlayerJumpState::~GPlayerJumpState()
{
}

void GPlayerJumpState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
	m_PlayerRigid = GetFSM()->GameObject()->RigidBody2D();
}

void GPlayerJumpState::Enter()
{
	m_Player->m_PlayerState = PLAYER_STATE::JUMP;

	if (!m_Player->FlipbookRender())
		return;

	m_Player->FlipbookRender()->Play((int)m_Player->m_PlayerState);

	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
	m_Player->RigidBody2D()->AddForce(Vector2(m_Player->m_KeyInput.HorizontalMove, 0) * m_Player->m_MoveInitForce);
}

void GPlayerJumpState::Tick()
{
	ChangeState();

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

void GPlayerJumpState::Exit()
{
}

void GPlayerJumpState::ChangeState()
{
	if (m_Player->m_KeyInput.HorizontalMove == 0)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
		return;
	}
}

void GPlayerJumpState::SaveToFile(FILE* _File)
{
}

void GPlayerJumpState::LoadFromFile(FILE* _File)
{
}
