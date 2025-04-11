#include "pch.h"
#include "GPlayerUseItemState.h"

#include <Engine/GTimeManager.h>

#include "GPlayer.h"
#include <Engine/GRigidBody2D.h>

#include "GPlatform.h"

GPlayerUseItemState::GPlayerUseItemState()
	: GScript(PLAYERUSEITEMSTATE)
	, m_Player(nullptr)
	, m_PlayerRigid(nullptr)
{

}

GPlayerUseItemState::~GPlayerUseItemState()
{

}

void GPlayerUseItemState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
	m_PlayerRigid = m_Player->RigidBody2D();

}

void GPlayerUseItemState::Enter()
{
	m_Player->m_PlayerState = PLAYER_STATE::USEITEM;

}

void GPlayerUseItemState::Tick()
{
	if (m_Player->m_PlayerUseItem == PLAYER_ITEM::HOOK)
	{
		Hook();
	}

}

void GPlayerUseItemState::Exit()
{
}

void GPlayerUseItemState::ChangeState()
{
}

void GPlayerUseItemState::Hook()
{
	m_PlayerRigid->AddForce(Vector2(0.f, m_PlayerRigid->GetFriction())* 2 * DT);

	// 속력이 m_MaxMoveSpeed보다 커지면 감속
	if (m_Player->m_HookMaxSpeed <= m_PlayerRigid->GetVelocity().y)
	{
		Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();
		PlayerSpeed.y = m_Player->m_HookMaxSpeed;
		m_PlayerRigid->SetVelocity(PlayerSpeed);
	}
	
}

void GPlayerUseItemState::OnTriggerEnter(GCollider2D* _Other)
{
	// Hook 사용 상태에서 Wood Platform에 닿았다면 변경

	if (m_Player->m_PlayerUseItem == PLAYER_ITEM::HOOK)
	{
		if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
			return;

		GPlatform* pPlatform = _Other->GameObject()->GetComponent<GPlatform>();
		assert(pPlatform);

		if (pPlatform->GetPlatformType() == PLATFORM_TYPE::WOOD)
		{
			m_Player->m_PlayerUseItem = PLAYER_ITEM::END;
			GetFSM()->ChanageState(L"Default");
		}
	}
	


}


void GPlayerUseItemState::SaveToFile(FILE* _File)
{
}

void GPlayerUseItemState::LoadFromFile(FILE* _File)
{
}