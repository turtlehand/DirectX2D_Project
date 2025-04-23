#include "pch.h"
#include "GItem.h"

#include <Engine/GCollider2D.h>
#include <Engine/GRigidBody2D.h>

vector<string> GItem::ItemName = {
	"HOOK",
	"BOMB",
	"SHOVEL",
	"HUG",
	"SHOT_GUN",
	"SWORD",
	"SHIELD",
	"CHICKEN",
	"PRINCESS",
	"END"
};

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
}

void GItem::SaveToFile(FILE* _File)
{
}

void GItem::LoadFromFile(FILE* _File)
{
}


