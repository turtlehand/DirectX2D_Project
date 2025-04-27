#include "pch.h"
#include "GEndingTimer.h"

#include "GGameManager.h"
#include <Engine/GTimeManager.h>

#include <Engine/components.h>
#include "GPlayer.h"

GEndingTimer::GEndingTimer()
	:GEndingTrigger(ENDINGTIMER)
	, m_Player(nullptr)
	, m_Duration(30.f)
	, m_Timer(0.f)
	, m_SucessEnd(false)
{
}

GEndingTimer::GEndingTimer(const GEndingTimer& _Origin)
	: GEndingTrigger(_Origin, ENDINGTIMER)
	, m_Player(nullptr)
	, m_Duration(30.f)
	, m_Timer(0.f)
	, m_SucessEnd(false)
{
}

GEndingTimer::~GEndingTimer()
{
}

void GEndingTimer::Init()
{
	GEndingTrigger::Init();
	ADD_FLOAT("Duration",&m_Duration);
	ADD_FLOAT("Timer", &m_Timer);
}

void GEndingTimer::Update()
{
	if (!m_Player || m_SucessEnd)
		return;

	if (m_Duration < m_Timer)
	{
		GGameManager::GetInst()->GameEnding(m_EndingType);
		m_SucessEnd = true;
	}
	else
	{
		m_Timer += DT;
	}
}

void GEndingTimer::OnOverlapEnter(GCollider2D* _Other)
{
	if (GGameManager::GetInst()->GetPlayType() != PLAY_TYPE::PLAY)
		return;

	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER)
	{
		m_Player = _Other->GameObject()->GetComponent<GPlayer>();
		m_Timer = 0.f;
		assert(m_Player);
	}
}

void GEndingTimer::OnOverlapExit(GCollider2D* _Other)
{
	if (GGameManager::GetInst()->GetPlayType() != PLAY_TYPE::PLAY)
		return;

	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER)
	{
		GPlayer* Player = _Other->GameObject()->GetComponent<GPlayer>();
		assert(m_Player == Player);
		m_Player = nullptr;
	}
}

void GEndingTimer::SaveToFile(FILE* _File)
{
	GEndingTrigger::SaveToFile(_File);
}

void GEndingTimer::LoadFromFile(FILE* _File)
{
	GEndingTrigger::LoadFromFile(_File);
}


