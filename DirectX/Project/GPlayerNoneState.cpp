#include "pch.h"
#include "GPlayerNoneState.h"

#include "GPlayer.h"

#include <Engine/GFlipbook.h>
#include <Engine/components.h>

GPlayerNoneState::GPlayerNoneState()
{
}

GPlayerNoneState::~GPlayerNoneState()
{
}

void GPlayerNoneState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
}

void GPlayerNoneState::Enter(DWORD_PTR _FlipBook)
{
	m_Player->m_PlayerState = PLAYER_STATE::NONE;
	PLAYER_FLIPBOOK Flipbook = (PLAYER_FLIPBOOK)_FlipBook;

	if (Flipbook != PLAYER_FLIPBOOK::END)
		m_Player->FlipbookRender()->Play((int)Flipbook);

}

void GPlayerNoneState::Tick()
{
}

void GPlayerNoneState::Exit()
{
}

void GPlayerNoneState::OnOverlapEnter(GCollider2D* _Other)
{
}

void GPlayerNoneState::OnOverlapStay(GCollider2D* _Other)
{
}

void GPlayerNoneState::OnOverlapExit(GCollider2D* _Other)
{
}
