#include "pch.h"
#include "GEndingRandom.h"

#include <Engine/components.h>

GEndingRandom::GEndingRandom()
	: GEndingTrigger(ENDINGRANDOM)
	, m_Prob(0.1f)
{
}

GEndingRandom::GEndingRandom(const GEndingRandom& _Origin)
	: GEndingTrigger(_Origin, ENDINGRANDOM)
	, m_Prob(_Origin.m_Prob)
{
}

GEndingRandom::~GEndingRandom()
{
}


void GEndingRandom::Init()
{
	ADD_ENUM("Ending Type", &m_EndingType, (DWORD_PTR)&EndingName);
	ADD_FLOAT("Probability", &m_Prob);
}

void GEndingRandom::Update()
{

}

void GEndingRandom::OnOverlapEnter(GCollider2D* _Other)
{
	if (GGameManager::GetInst()->GetPlayType() != PLAY_TYPE::PLAY)
		return;

	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER)
	{
		if (m_Prob * 100 > rand() % 100)
		{
			GGameManager::GetInst()->GameEnding(m_EndingType);
		}
	}
}

void GEndingRandom::SaveToFile(FILE* _File)
{
	GEndingTrigger::SaveToFile(_File);
	fwrite(&m_Prob, sizeof(float), 1, _File);
}

void GEndingRandom::LoadFromFile(FILE* _File)
{
	GEndingTrigger::LoadFromFile(_File);
	fread(&m_Prob, sizeof(float), 1, _File);
}

