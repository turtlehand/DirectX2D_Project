#include "pch.h"
#include "GPlayerWalkState.h"

#include "GPlayer.h"
#include <Engine/GFlipbookRender.h>

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
}

void GPlayerWalkState::Enter()
{
	m_Player->m_PlayerState = PLAYER_STATE::WALK;

	if (!m_Player->FlipbookRender())
		return;

	m_Player->FlipbookRender()->Play((int)m_Player->m_PlayerState);
}

void GPlayerWalkState::Tick()
{
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
