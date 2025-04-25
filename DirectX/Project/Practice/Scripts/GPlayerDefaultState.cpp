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

	// �⺻ �ø��� ���
	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::DEFAULT);
}

void GPlayerDefaultState::Tick()
{
	// ������ ��������
	if (!m_Player->m_IsGround)
	{
		m_Player->GetFSM()->ChanageState(L"Fall");
		return;
	}

	// ������ ������ ����� �����ϴٸ�
	if (m_Player->m_KeyInput.Interaction)
	{
		if (m_Player->Interaction())
		{
			m_Player->GetFSM()->ChanageState(L"UseItem");
			return;
		}
	}

	// �̵��ϱ� �����ߴٸ�
	if (m_Player->m_KeyInput.HorizontalMove != 0)
	{
		m_Player->GetFSM()->ChanageState(L"Walk");
		return;
	}

	// ����Ű�� �����ٸ�
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


