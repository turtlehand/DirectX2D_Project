#include "pch.h"
#include "GPlayerJumpState.h"

#include "GGameManager.h"

#include "GPlayer.h"
#include <Engine/GFlipbookRender.h>
#include <Engine/GRigidBody2D.h>
#include <Engine/GTransform.h>
#include <Engine/GTimeManager.h>
#include <Engine/GAssetManager.h>
#include <Engine/GSound.h>
#include "GGameManager.h"

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
	m_JumpSound = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\Jump.wav", L"Sound\\AudioClip\\Jump.wav");
}

void GPlayerJumpState::Enter()
{
	//GGameManager::GetInst()->GameEnding(ENDING_TYPE::Older_Man_Attack);
	//GTimeManager::GetInst()->SetTimeScale(0.1f);

	m_Player->m_PlayerState = PLAYER_STATE::JUMP;

	if (!m_Player->FlipbookRender())
		return;

	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::JUMP);

	// ���� ����
	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);
	
	// �ʱ� ��
	m_Player->RigidBody2D()->SetVelocityY(m_Player->m_JumpMaxSpeed);

	m_Player->m_JumpTimer = 0.f;

	m_JumpSound->Play(1, GGameManager::GetInst()->GetEffect_Volume(), true);
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

	// õ�忡 ������ ��� �ϰ��Ѵ�.
	if (m_Player->m_IsCeiling > 0)
	{
		m_Player->GetFSM()->ChanageState(L"Fall");
		return;
	}

	// ������ �����ϰ� ������ �ּ� �ð� �̻� �ִ� �ð� ���Ϸ� �����ȴ�.
	if (!m_Player->m_KeyInput.JumpHold) 
	{
		// �ּ� �ð��� �����Ѵ�.
		if (m_Player->m_JumpTimer >= m_Player->m_JumpTimeMin)
		{
			m_Player->GetFSM()->ChanageState(L"Fall");
			return;
		}
	}
	else
	{
		// �ִ� �ð��� �����Ͽ���.
		if (m_Player->m_JumpTimer > m_Player->m_JumpTimeLimit)
		{
			m_Player->GetFSM()->ChanageState(L"Fall");
			return;

		}
	}


	m_Player->m_JumpTimer += FDT;

	assert(m_PlayerRigid);

	

	// ���� ���� ��ȯ
	// �ٶ� ���� ���� ����
	m_Player->SetMoveDirection(m_Player->m_KeyInput.HorizontalMove);

	// ������ ����
	m_PlayerRigid->AddForce(
		Vector2(m_Player->m_KeyInput.HorizontalMove * m_PlayerRigid->GetFriction() * 2
			, m_PlayerRigid->GetFriction() * 4) * FDT);
	Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();

	// �ӷ��� m_MaxMoveSpeed���� Ŀ���� ����
	if (m_Player->m_MoveMaxSpeed < fabs(m_PlayerRigid->GetVelocity().x))
	{
		PlayerSpeed.x = m_Player->m_MoveMaxSpeed * m_Player->m_KeyInput.HorizontalMove;
	}

	// ���� �ӷ� ����
	// ������ �Ѱ� �� 25% �ӷ��� ����
	float JumpMaxSpeed = m_Player->m_JumpMaxSpeed;
	for (int i = 0; i < (int)PLAYER_ITEM::END; ++i)
	{
		// ���� �ӵ��� 0���϶�� ����
		if (JumpMaxSpeed <= 0.f)
		{
			JumpMaxSpeed = 0;
			break;
		}
		if (!m_Player->m_PlayerItems[i])
			continue;

		JumpMaxSpeed -= m_Player->m_JumpMaxSpeed / (m_Player->m_ItemMaxCount * 2.f);
	}
	
	if (JumpMaxSpeed < m_PlayerRigid->GetVelocity().y)
	{
		PlayerSpeed.y = JumpMaxSpeed;
	}

	m_PlayerRigid->SetVelocity(PlayerSpeed);
}

void GPlayerJumpState::Exit()
{
	//GTimeManager::GetInst()->SetTimeScale(1.f);
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
