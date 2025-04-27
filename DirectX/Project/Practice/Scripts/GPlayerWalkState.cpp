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

	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::WALK);

	// 캐릭터의 바라보고 있는 방향을 바꾼다.
	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
	
	// 처음에만 큰 힘을 줘서 빠르게 최대 속도에 도달하게 한다.
	m_Player->RigidBody2D()->SetVelocityX(m_Player->m_MoveMaxSpeed* m_Player->m_KeyInput.HorizontalMove);
}

void GPlayerWalkState::Tick()
{
	// 땅에서 떨어졌다면
	if (!m_Player->m_IsGround)
	{
		m_JumpBonusTimer += DT;
		if (0.2f < m_JumpBonusTimer)
		{
			m_Player->GetFSM()->ChanageState(L"Fall");
			return;
		}

	}
	else
		m_JumpBonusTimer = 0.f;

	// 아이템 사용이 가능하다면
	if (m_Player->m_KeyInput.Interaction)
	{
		if (m_Player->Interaction())
		{
			m_Player->GetFSM()->ChanageState(L"UseItem");
			return;
		}
	}

	// 이동을 멈추었다면
	if (m_Player->m_KeyInput.HorizontalMove == 0)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
		return;
	}
	
	// 점프를 하기 시작했다면
	if (m_Player->m_KeyInput.Jump)
	{
		m_Player->GetFSM()->ChanageState(L"Jump");
		return;
	}

	assert(m_PlayerRigid);

	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
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
