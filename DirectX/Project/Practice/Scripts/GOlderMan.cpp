#include "pch.h"
#include "GOlderMan.h"

#include "GGameManager.h"
#include <Engine/GAssetManager.h>
#include <Engine/components.h>

GOlderMan::GOlderMan()
	:GObjectBasic(OLDERMAN)
{
}

GOlderMan::~GOlderMan()
{
}

void GOlderMan::Init()
{
}

void GOlderMan::Begin()
{
}

void GOlderMan::Update()
{
}

void GOlderMan::OnOverlapEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER_ATTACK)
	{
		GGameObject* pBlood = GAssetManager::GetInst()->FindAsset<GPrefab>(L"Prefab\\Blood.prefab")->Instantiate();
		SpawnGameObject(pBlood);
		GameObject()->SetChild(pBlood);
		GGameManager::GetInst()->GameEnding(ENDING_TYPE::Older_Man_Attack);
	}
}

void GOlderMan::SaveToFile(FILE* _File)
{
}

void GOlderMan::LoadFromFile(FILE* _File)
{
}
