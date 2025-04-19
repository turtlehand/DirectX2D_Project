#include "pch.h"
#include "GPlayerFallState.h"

#include <Engine/GTimeManager.h>
#include <Engine/GTransform.h>

#include <Engine/GFlipbook.h>
#include <Engine/GFlipbookRender.h>

#include <Engine/GRigidBody2D.h>
#include "GPlayer.h"

GPlayerFallState::GPlayerFallState()
	:GScript(PLAYERFALLSTATE)
{
}

GPlayerFallState::~GPlayerFallState()
{
}

void GPlayerFallState::SaveToFile(FILE* _File)
{
}

void GPlayerFallState::LoadFromFile(FILE* _File)
{
}

void GPlayerFallState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
	m_PlayerRigid = GetFSM()->GameObject()->RigidBody2D();
}

void GPlayerFallState::Enter()
{
	m_Player->m_PlayerState = PLAYER_STATE::FALL;

	//m_PlayerRigid->SetGravity(m_Player->m_GravityScale);
}

void GPlayerFallState::Tick()
{
	if (m_Player->FlipbookRender()->GetCurIndex() != (int)PLAYER_FLIPBOOK::FALL)
	{
		if (m_PlayerRigid->GetVelocity().y < 0)
			m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::FALL);
	}

	if (m_Player->m_KeyInput.Interaction)
	{
		if (m_Player->Interaction())
		{
			m_Player->GetFSM()->ChanageState(L"UseItem");
			return;
		}
	}

	if (m_Player->m_IsGround)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
		return;
	}

	Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();

	// 공중 방향 전환
	// 바라 보는 방향 설정
	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);

	// 움직임 설정
	m_PlayerRigid->AddForce(
		Vector2(m_Player->m_KeyInput.HorizontalMove * m_PlayerRigid->GetFriction() * 2
			, 0.f) * DT);

	// 속력이 m_MaxMoveSpeed보다 커지면 감속
	if (m_Player->m_MoveMaxSpeed < fabs(m_PlayerRigid->GetVelocity().x))
	{
		PlayerSpeed.x = m_Player->m_MoveMaxSpeed * m_Player->m_KeyInput.HorizontalMove;
	}

	if ( -m_Player->m_JumpMaxSpeed > m_PlayerRigid->GetVelocity().y)
	{
		PlayerSpeed.y = -m_Player->m_JumpMaxSpeed;
	}

	m_PlayerRigid->SetVelocity(PlayerSpeed);
}

void GPlayerFallState::Exit()
{
	//m_PlayerRigid->SetVelocity(Vector2(m_PlayerRigid->GetVelocity().x, 0.f));
	//m_PlayerRigid->SetGravity(0);
}

void GPlayerFallState::OnOverlapEnter(GCollider2D* _Other)
{

}

void GPlayerFallState::OnOverlapStay(GCollider2D* _Other)
{
}

void GPlayerFallState::OnOverlapExit(GCollider2D* _Other)
{
}

void GPlayerFallState::ChangeState()
{
}


