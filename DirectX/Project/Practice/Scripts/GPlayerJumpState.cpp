#include "pch.h"
#include "GPlayerJumpState.h"

#include "GGameManager.h"

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
	//GGameManager::GetInst()->GameEnding(ENDING_TYPE::Older_Man_Attack);

	m_Player->m_PlayerState = PLAYER_STATE::JUMP;

	if (!m_Player->FlipbookRender())
		return;

	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::JUMP);

	// 방향 변경
	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
	
	// 초기 힘
	m_Player->RigidBody2D()->AddForce(
		Vector2(0.f
				, m_Player->m_JumpPower));

	m_Player->m_JumpTimer = 0.f;
}

void GPlayerJumpState::Tick()
{
	if (m_Player->m_KeyInput.Interaction)
	{
		if (m_Player->Interaction())
		{
			m_Player->GetFSM()->ChanageState(L"UseItem");
			return;
		}
	}

	// 천장에 닿으면 즉시 하강한다.
	if (m_Player->m_IsCeiling > 0)
	{
		m_Player->GetFSM()->ChanageState(L"Fall");
		return;
	}

	// 점프를 시작하고 점프는 최소 시간 이상 최대 시간 이하로 유지된다.
	if (!m_Player->m_KeyInput.JumpHold) 
	{
		// 최소 시간은 점프한다.
		if (m_Player->m_JumpTimer >= m_Player->m_JumpTimeMin)
		{
			m_Player->GetFSM()->ChanageState(L"Fall");
			return;
		}
	}
	else
	{
		// 최대 시간을 점프하였다.
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

	// 점프 속력 제한
	// 아이템 한개 당 25% 속력이 감소
	float JumpMaxSpeed = m_Player->m_JumpMaxSpeed;
	for (int i = 0; i < (int)PLAYER_ITEM::END; ++i)
	{
		// 점프 속도가 0이하라면 종료
		if (JumpMaxSpeed <= 0.f)
		{
			JumpMaxSpeed = 0;
			break;
		}
		if (!m_Player->m_PlayerItems[i])
			continue;

		JumpMaxSpeed -= m_Player->m_JumpMaxSpeed / m_Player->m_ItemMaxCount;
	}
	
	if (JumpMaxSpeed < m_PlayerRigid->GetVelocity().y)
	{
		PlayerSpeed.y = JumpMaxSpeed;
	}

	m_PlayerRigid->SetVelocity(PlayerSpeed);
}

void GPlayerJumpState::Exit()
{
	
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
