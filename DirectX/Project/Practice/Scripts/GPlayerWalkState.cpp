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

	// ĳ������ �ٶ󺸰� �ִ� ������ �ٲ۴�.
	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
	
	// ó������ ū ���� �༭ ������ �ִ� �ӵ��� �����ϰ� �Ѵ�.
	m_Player->RigidBody2D()->SetVelocityX(m_Player->m_MoveMaxSpeed* m_Player->m_KeyInput.HorizontalMove);
}

void GPlayerWalkState::Tick()
{
	// ������ �������ٸ�
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

	// ������ ����� �����ϴٸ�
	if (m_Player->m_KeyInput.Interaction)
	{
		if (m_Player->Interaction())
		{
			m_Player->GetFSM()->ChanageState(L"UseItem");
			return;
		}
	}

	// �̵��� ���߾��ٸ�
	if (m_Player->m_KeyInput.HorizontalMove == 0)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
		return;
	}
	
	// ������ �ϱ� �����ߴٸ�
	if (m_Player->m_KeyInput.Jump)
	{
		m_Player->GetFSM()->ChanageState(L"Jump");
		return;
	}

	assert(m_PlayerRigid);

	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
	m_PlayerRigid->AddForce(Vector2(m_Player->m_KeyInput.HorizontalMove, 0) * m_PlayerRigid->GetFriction() * 2 * DT);

	// �ӷ��� m_MaxMoveSpeed���� Ŀ���� ����
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
