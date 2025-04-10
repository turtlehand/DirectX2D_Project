#include "pch.h"
#include "GPlayerDefaultState.h"

#include "GPlayer.h"
#include <Engine/GFlipbookRender.h>

GPlayerDefaultState::GPlayerDefaultState()
	:GScript(PLAYERDEFAULTSTATE)
	, m_Player(nullptr)
{
}

GPlayerDefaultState::~GPlayerDefaultState()
{
}

void GPlayerDefaultState::SaveToFile(FILE* _File)
{
}

void GPlayerDefaultState::LoadFromFile(FILE* _File)
{
}

void GPlayerDefaultState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
}

void GPlayerDefaultState::Enter()
{
	m_Player->m_PlayerState = PLAYER_STATE::DEFAULT;

	if (!m_Player->FlipbookRender())
		return;

	m_Player->FlipbookRender()->Play((int)m_Player->m_PlayerState);
}

void GPlayerDefaultState::Tick()
{

}

void GPlayerDefaultState::Exit()
{

}


