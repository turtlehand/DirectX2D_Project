#include "pch.h"
#include "GEndingNeedItem.h"

#include "GItem.h"
#include "GPlayer.h"
#include <Engine/components.h>

GEndingNeedItem::GEndingNeedItem()
	: GEndingTrigger(ENDINGNEEDITEM)
	, m_Item(PLAYER_ITEM::PRINCESS)
{
}

GEndingNeedItem::GEndingNeedItem(const GEndingNeedItem& _Origin)
	: GEndingTrigger(_Origin, ENDINGNEEDITEM)
	, m_Item(_Origin.m_Item)
{
}

GEndingNeedItem::~GEndingNeedItem()
{
}


void GEndingNeedItem::Init()
{
	GEndingTrigger::Init();
	ADD_ENUM("Item", &m_Item, (DWORD_PTR)&ItemName);
}

void GEndingNeedItem::Update()
{
}

void GEndingNeedItem::OnOverlapEnter(GCollider2D* _Other)
{
	if (GGameManager::GetInst()->GetPlayType() != PLAY_TYPE::PLAY)
		return;

	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER)
	{
		GPlayer* Player = _Other->GameObject()->GetComponent<GPlayer>();
		assert(Player);

		if(Player->GetItem(m_Item))
			GGameManager::GetInst()->GameEnding(m_EndingType);
	}
}

void GEndingNeedItem::SaveToFile(FILE* _File)
{
	GEndingTrigger::SaveToFile(_File);
}

void GEndingNeedItem::LoadFromFile(FILE* _File)
{
	GEndingTrigger::LoadFromFile(_File);
}

