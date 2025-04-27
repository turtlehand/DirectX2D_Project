#include "pch.h"
#include "GItem.h"

#include <Engine/GCollider2D.h>
#include <Engine/GRigidBody2D.h>

GItem::GItem()
	:GObjectBasic(ITEM)
	, m_ItemType(PLAYER_ITEM::END)
{
}

GItem::~GItem()
{
}

void GItem::Init()
{
	ADD_ENUM("Item_Type", &m_ItemType, (DWORD_PTR)&ItemName);
}

void GItem::Update()
{
	GObjectBasic::Update();
}

void GItem::SaveToFile(FILE* _File)
{
	fwrite(&m_ItemType, sizeof(PLAYER_ITEM), 1, _File);
}

void GItem::LoadFromFile(FILE* _File)
{
	fread(&m_ItemType, sizeof(PLAYER_ITEM), 1, _File);
}


