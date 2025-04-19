#include "pch.h"
#include "GPlayerGetItemState.h"

#include <Engine/GTimeManager.h>

#include "GPlayer.h"
#include "GItem.h"
#include <Engine/components.h>

GPlayerGetItemState::GPlayerGetItemState()
	: m_Player(nullptr)
	, m_PlayerRigid(nullptr)
	, m_Timer(0.f)
{
}

GPlayerGetItemState::~GPlayerGetItemState()
{
}

void GPlayerGetItemState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
	m_PlayerRigid = GetFSM()->GameObject()->RigidBody2D();
}

void GPlayerGetItemState::Enter(DWORD_PTR _Item)
{
	m_Item = (GItem*)_Item;

	m_Item->GameObject()->SetParent(m_Player->GameObject());
	m_Item->Collider2D()->Destroy();
	m_Item->RigidBody2D()->Destroy();
	m_Item->Transform()->SetRelativeScale(m_Item->Transform()->GetRelativeScale() / m_Player->Transform()->GetRelativeScale());

	m_Player->m_PlayerState = PLAYER_STATE::GETITEM;
	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::GETITEM);
	m_Timer = 0.f;
}

void GPlayerGetItemState::Tick()
{
	Vector3 Pos;
	if (m_Timer < 1.f)
	{
		Pos = Vector3(0.f, 1.f, 0.5f) * (m_Timer)+Vector3(0.0f, 0.0f, 0.5f) * (1 - m_Timer);
	}
	else
		Pos = Vector3(0.f, 1.f, 0.5f);
	
	m_Item->Transform()->SetRelativePos(Pos);

	if (2.f < m_Timer)
	{
		m_Player->m_FSM->ChanageState(L"Default");
		return;
	}
	m_Timer += DT;
}

void GPlayerGetItemState::Exit()
{
	int Index = (int)m_Item->GetItemType();
	m_Player->m_PlayerItems[Index] = true;
	m_Item->Transform()->SetRelativePos(Vector3(-0.25f, 0.f, 0.5f));
	//m_Item->Transform()->SetRelativeScale(Vector3(1.f, 1.f, 1.f));

}

void GPlayerGetItemState::OnTriggerEnter(GCollider2D* _Other)
{
}

void GPlayerGetItemState::OnTriggerStay(GCollider2D* _Other)
{
}

void GPlayerGetItemState::OnTriggerExit(GCollider2D* _Other)
{
}


