#include "pch.h"
#include "GMinionWalkState.h"

#include "GMinion.h"
#include <Engine/components.h>

#include <Engine/GTimeManager.h>
#include <Engine/GCollisionManager.h>

GMinionWalkState::GMinionWalkState()
{
}

GMinionWalkState::~GMinionWalkState()
{
}

void GMinionWalkState::Awake()
{
	m_Minion = GetFSM()->GameObject()->GetComponent<GMinion>();
	m_MinionRigid = GetFSM()->GameObject()->RigidBody2D();
}

void GMinionWalkState::Enter()
{
	m_Minion->m_MinionState = MINION_STATE::WALK;

	if (!m_Minion->FlipbookRender())
		return;

	m_Minion->FlipbookRender()->Play((int)MINION_STATE::WALK);

	// �̹����� �����̴� ������ �ٶ󺸰� ����
	m_Minion->SetMoveDirection(m_Minion->m_HorizontalMove);
	
	// �ʱ���
	m_Minion->RigidBody2D()->AddForce(Vector2(m_Minion->m_HorizontalMove, 0) * m_Minion->m_MoveInitForce);

}

void GMinionWalkState::Tick()
{
	Vector3 Pos = m_Minion->Transform()->GetWorldPos();
	Vector3 Scale = m_Minion->Transform()->GetWorldScale();

	// �÷��̾� Ž��
	DrawDebugLine(Vector4(1.f, 0.f, 0.f, 1.f), Vector3(Pos.x , Pos.y, 0.f), Vector3(1.f, 0.f, 0.f), Scale.x / 2);

	if (GCollisionManager::GetInst()->Line_Casting(Vector3(Pos.x , Pos.y, Pos.z)
		, Vector3(1.f, 0.f, 0.f)
		, Scale.x / 2
		, (1 << (int)LAYER_TYPE::PLAYER)))
	{
		m_Minion->m_FSM->ChanageState(L"Attack");
		return;
	}

	// ���� ��ȯ 
	DrawDebugLine(Vector4(0.f, 1.f, 0.f, 1.f), Vector3(Pos.x + Scale.x / 4, Pos.y, 0.f), Vector3(0.f, -1.f, 0.f), Scale.y);

	if (!GCollisionManager::GetInst()->Line_Casting(Vector3(Pos.x + Scale.x / 4, Pos.y, Pos.z)
		, Vector3(0.f, -1.f, 0.f)
		, Scale.y
		, (1 << (int)LAYER_TYPE::PLATFORM)))
	{
		m_Minion->m_HorizontalMove = m_Minion->m_HorizontalMove * -1;
		m_MinionRigid->SetVelocityX(0.f);
		m_Minion->SetMoveDirection(m_Minion->m_HorizontalMove);
		m_Minion->RigidBody2D()->AddForce(Vector2(m_Minion->m_HorizontalMove, 0) * m_Minion->m_MoveInitForce);
	}

	m_MinionRigid->AddForce(Vector2(m_Minion->m_HorizontalMove, 0) * m_MinionRigid->GetFriction() * 2 * DT);


	// �ӷ��� m_MaxMoveSpeed���� Ŀ���� ����
	if (m_Minion->m_MoveMaxSpeed < fabs(m_MinionRigid->GetVelocity().x))
	{
		m_MinionRigid->SetVelocityX(m_Minion->m_MoveMaxSpeed * m_Minion->m_HorizontalMove);
	}

}

void GMinionWalkState::Exit()
{
}


