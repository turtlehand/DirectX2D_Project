#include "pch.h"
#include "PrefabUI.h"

void PrefabUI::Init()
{
}

void PrefabUI::Update_UI()
{
}

void PrefabUI::Render_UI()
{
	OutputTitle("Prefab");
	OutputKeyPath();
}

PrefabUI::PrefabUI()
	:AssetUI(ASSET_TYPE::PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}
