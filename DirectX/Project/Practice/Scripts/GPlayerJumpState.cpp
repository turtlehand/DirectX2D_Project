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
	m_Player->RigidBody2D()->AddForce(
		Vector2(m_Player->m_KeyInput.HorizontalMove * m_Player->m_MoveInitForce
				, m_Player->m_JumpPower));

	m_Player->m_JumpTimer = 0.f;
}

void GPlayerJumpState::Tick()
{

	if (!m_Player->m_KeyInput.Jump) 
	{
		if (m_Player->m_JumpTimer >= m_Player->m_JumpTimeMin)
		{
			m_Player->GetFSM()->ChanageState(L"Fall");
			return;
		}
	}
	else
	{
		if (m_Player->m_JumpTimer > m_Player->m_JumpTimeLimit)
		{
			m_Player->GetFSM()->ChanageState(L"Fall");
			return;

		}
	}


	m_Player->m_JumpTimer += DT;

	assert(m_PlayerRigid);

	Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();

	// 공중 방향 전환
	// 바라 보는 방향 설정
	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);

	// 움직임 설정
	m_PlayerRigid->AddForce(
		Vector2(m_Player->m_KeyInput.HorizontalMove * m_PlayerRigid->GetFriction() * 2
			, m_PlayerRigid->GetFriction() * 4) * DT);

	// 속력이 m_MaxMoveSpeed보다 커지면 감속
	if (m_Player->m_MoveMaxSpeed < fabs(m_PlayerRigid->GetVelocity().x))
	{
		PlayerSpeed.x = m_Player->m_MoveMaxSpeed * m_Player->m_KeyInput.HorizontalMove;
	}

	if (m_Player->m_JumpMaxSpeed < m_PlayerRigid->GetVelocity().y)
	{
		PlayerSpeed.y = m_Player->m_JumpMaxSpeed;
	}

	m_PlayerRigid->SetVelocity(PlayerSpeed);
}

void GPlayerJumpState::Exit()
{
	m_PlayerRigid->SetVelocity(Vector2(m_PlayerRigid->GetVelocity().x, 0.f));
}

void GPlayerJumpState::ChangeState()
{

}

void GPlayerJumpState::SaveToFile(FILE* _File)
{
}

void GPlayerJumpState::LoadFromFile(FILE* _File)
{
}
