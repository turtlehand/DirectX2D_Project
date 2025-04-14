#include "pch.h"
#include "GPlayerFallState.h"

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

	if (!m_Player->FlipbookRender())
		return;

	m_Player->FlipbookRender()->Play((int)m_Player->m_PlayerState);

	m_PlayerRigid->SetGravity(m_Player->m_GravityScale);
}

void GPlayerFallState::Tick()
{
	if (m_Player->m_IsGround)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
	}
}

void GPlayerFallState::Exit()
{
	m_PlayerRigid->SetVelocity(Vector2(m_PlayerRigid->GetVelocity().x, 0.f));
	m_PlayerRigid->SetGravity(0);
}

void GPlayerFallState::OnTriggerEnter(GCollider2D* _Other)
{

}

void GPlayerFallState::OnTriggerStay(GCollider2D* _Other)
{
}

void GPlayerFallState::OnTriggerExit(GCollider2D* _Other)
{
}

void GPlayerFallState::ChangeState()
{
}


