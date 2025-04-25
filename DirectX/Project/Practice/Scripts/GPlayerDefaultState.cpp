#include "pch.h"
#include "GPlayerDefaultState.h"

#include "GPlayer.h"
#include <Engine/GFlipbookRender.h>

#include <Engine/GTimeManager.h>

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

	// 기본 플리북 재생
	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::DEFAULT);
}

void GPlayerDefaultState::Tick()
{
	// 땅에서 떨어지면
	if (!m_Player->m_IsGround)
	{
		m_Player->GetFSM()->ChanageState(L"Fall");
		return;
	}

	// 적절한 아이템 사용이 가능하다면
	if (m_Player->m_KeyInput.Interaction)
	{
		if (m_Player->Interaction())
		{
			m_Player->GetFSM()->ChanageState(L"UseItem");
			return;
		}
	}

	// 이동하기 시작했다면
	if (m_Player->m_KeyInput.HorizontalMove != 0)
	{
		m_Player->GetFSM()->ChanageState(L"Walk");
		return;
	}

	// 점프키를 눌렀다면
	if (m_Player->m_KeyInput.Jump)
	{
		m_Player->GetFSM()->ChanageState(L"Jump");
		return;
	}

}

void GPlayerDefaultState::Exit()
{

}

void GPlayerDefaultState::ChangeState()
{

}


