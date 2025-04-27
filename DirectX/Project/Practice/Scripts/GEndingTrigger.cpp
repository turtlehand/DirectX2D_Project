#include "pch.h"
#include "GEndingTrigger.h"

#include <Engine/components.h>

GEndingTrigger::GEndingTrigger(SCRIPT_TYPE _Type)
	: GScript(_Type)
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
	fwrite(&m_EndingType, sizeof(ENDING_TYPE), 1, _File);
}

void GEndingTrigger::LoadFromFile(FILE* _File)
{
	fread(&m_EndingType, sizeof(ENDING_TYPE), 1, _File);
}


