#include "pch.h"
#include "GPlayerGetItemState.h"

#include <Engine/GTimeManager.h>

#include "GPlayer.h"
#include "GItem.h"
#include "GPrincess.h"
#include <Engine/components.h>

#include <Engine/GAssetManager.h>
#include <Engine/GSound.h>

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
	m_GetItemSound = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\Item.wav", L"Sound\\AudioClip\\Item.wav");
}

void GPlayerGetItemState::Enter(DWORD_PTR _Item)
{
	m_Item = (GGameObject*)_Item;

	m_Item->SetParent(m_Player->GameObject());
	
	// 쓸모없는 컴포넌트 삭제
	m_Item->Collider2D()->Destroy();
	//m_Item->RigidBody2D()->Destroy();
	m_Item->Transform()->SetRelativeScale(m_Item->Transform()->GetRelativeScale() / m_Player->Transform()->GetRelativeScale());
	m_Item->Transform()->SetRelativeRotation(Vector3(0.f, 0.f, 0.f));

	// GroundChecker 삭게
	const vector<GGameObject*>& ItemChilds = m_Item->GetChild();
	for (int i = 0; i < ItemChilds.size(); ++i)
	{
		if (ItemChilds[i]->GetName() == L"GroundChecker")
		{
			ItemChilds[i]->Destroy();
			break;
		}

	}


	m_Player->m_PlayerState = PLAYER_STATE::GETITEM;
	m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::GETITEM);
	m_Timer = 0.f;

	m_GetItemSound->Play(1, GGameManager::GetInst()->GetEffect_Volume(), false);
}

void GPlayerGetItemState::Tick()
{
	Vector3 Pos;
	if (m_Timer < 1.f)
	{
		Pos = Vector3(0.f, 1.f, -2.f) * (m_Timer)+Vector3(0.0f, 0.0f, -2.f) * (1 - m_Timer);
	}
	else
		Pos = Vector3(0.f, 1.f, -2.f);
	
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
	int Index = (int)PLAYER_ITEM::END;
	GItem* ItemScript = m_Item->GetComponent<GItem>();
	GPrincess* PrincessScript = m_Item->GetComponent<GPrincess>();
	assert(ItemScript == nullptr || PrincessScript == nullptr);

	if (ItemScript)
	{
		Index = (int)ItemScript->GetItemType();
	}
	else if (PrincessScript)
	{
		Index = (int)PrincessScript->GetItemType();
		GGameManager::GetInst()->CallDarkLord();
		Ptr<GSound> Earthquake = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\EarthquakePrincessRescue.wav", L"Sound\\AudioClip\\EarthquakePrincessRescue.wav");

		Earthquake->Play(1, GGameManager::GetInst()->GetEffect_Volume(), false);
	}

	assert(Index != (int)PLAYER_ITEM::END);
	
	m_Player->m_PlayerItems[Index] = true;
	m_Item->Transform()->SetRelativePos(Vector3(-0.25f, 0.f, 0.5f));
	//m_Item->Transform()->SetRelativeScale(Vector3(1.f, 1.f, 1.f));

	int ItemCount = 0;
	for (int i = 0; i < (int)PLAYER_ITEM::END; ++i)
	{
		ItemCount += m_Player->m_PlayerItems[i] ? 1 : 0;
	}

	if (ItemCount >= m_Player->m_ItemMaxCount)
		GGameManager::GetInst()->GameEnding(ENDING_TYPE::Mundane_Pouch);

}

void GPlayerGetItemState::OnOverlapEnter(GCollider2D* _Other)
{
}

void GPlayerGetItemState::OnOverlapStay(GCollider2D* _Other)
{
}

void GPlayerGetItemState::OnOverlapExit(GCollider2D* _Other)
{
}


