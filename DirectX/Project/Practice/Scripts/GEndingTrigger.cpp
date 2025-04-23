#include "pch.h"
#include "GEndingTrigger.h"

#include <Engine/components.h>

GEndingTrigger::GEndingTrigger()
	: GScript(ENDINGTRIGGER)
	, m_EndingType(ENDING_TYPE::END)
{
}

GEndingTrigger::~GEndingTrigger()
{
}

void GEndingTrigger::Init()
{
	ADD_ENUM("Ending Type", &m_EndingType, (DWORD_PTR)&EndingName);
}

void GEndingTrigger::Update()
{
}

void GEndingTrigger::OnOverlapEnter(GCollider2D* _Other)
{
	if (GGameManager::GetInst()->GetPlayType() != PLAY_TYPE::PLAY)
		return;

	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER)
	{
		GGameManager::GetInst()->GameEnding(m_EndingType);
	}
}

void GEndingTrigger::SaveToFile(FILE* _File)
{
}

void GEndingTrigger::LoadFromFile(FILE* _File)
{
}


