#include "pch.h"
#include "GItem.h"

vector<string> GItem::ItemName = {
	"HOOK",
	"BOMB",
	"SHOVEL",
	"HUG",
	//SHOT_GUN,
	"SWORD",
	//SHIELD,
	"CHICKEN",
	"END"
};

GItem::GItem()
	:GScript(ITEM)
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


