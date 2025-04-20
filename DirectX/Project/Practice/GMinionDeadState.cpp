#include "pch.h"
#include "GMinionDeadState.h"

#include "GMinion.h"
#include <Engine/components.h>

#include "GGameManager.h"

GMinionDeadState::GMinionDeadState()
{
}

GMinionDeadState::~GMinionDeadState()
{
}

void GMinionDeadState::Awake()
{
	m_Minion = GetFSM()->GameObject()->GetComponent<GMinion>();
}

void GMinionDeadState::Enter()
{
	m_Minion->m_MinionState = MINION_STATE::DEAD;

	m_Minion->FlipbookRender()->Play((int)MINION_STATE::DEAD);
}

void GMinionDeadState::Tick()
{
}

void GMinionDeadState::Exit()
{
}


