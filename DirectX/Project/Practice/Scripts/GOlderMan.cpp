#include "pch.h"
#include "GOlderMan.h"

#include <Engine/GTimeManager.h>

#include "GGameManager.h"
#include <Engine/GAssetManager.h>
#include <Engine/GSound.h>
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
	ADD_FLOAT("Timer", &m_Timer);
}

void GOlderMan::Begin()
{
}

void GOlderMan::Update()
{
	GObjectBasic::Update();
	if (m_IsDead)
	{
		if (0 < m_Timer && m_Timer < 0.1f)
		{
			float ratio = m_Timer / 0.1;
			Transform()->SetRelativeRotation(0.f, 0.f, 90 * ratio);
		}
		else if (0.1f < m_Timer && m_Timer < 0.2f)
		{
			float ratio = (m_Timer - 0.1f) / 0.1;
			Transform()->SetRelativeRotation(0.f, 0.f, 90.f * (1- ratio) + 75.f * ratio);
		}
		else if (0.2f < m_Timer && m_Timer < 0.375f)
		{
			float ratio = (m_Timer - 0.2f) / 0.175f;
			Transform()->SetRelativeRotation(0.f, 0.f, 90.f * ratio + 75.f * (1 - ratio));
		}
		else if (0.375 < m_Timer)
		{
			Transform()->SetRelativeRotation(0.f, 0.f, 90);
		}
		m_Timer += DT;
	}
}

void GOlderMan::OnOverlapEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER_ATTACK)
	{
		// 피 파티클 생성
		GGameObject* pBlood = GAssetManager::GetInst()->Load<GPrefab>(L"Prefab\\Blood.prefab", L"Prefab\\Blood.prefab")->Instantiate();
		SpawnGameObject(pBlood);
		GameObject()->SetChild(pBlood);

		// 죽음
		if (!m_IsDead)
		{
			m_IsDead = true;
			m_Timer = 0.f;

			Ptr<GSound> Stab = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\StabSomeone.wav", L"Sound\\AudioClip\\StabSomeone.wav");
			Ptr<GSound> KillElder = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\KillElder.wav", L"Sound\\AudioClip\\KillElder.wav");

			Stab->Play(1, GGameManager::GetInst()->GetEffect_Volume(), false);
			KillElder->Play(1, GGameManager::GetInst()->GetEffect_Volume(), false);

			// 엔딩
			GGameManager::GetInst()->GameEnding(ENDING_TYPE::Older_Man_Attack);
		}

		
	}
}

void GOlderMan::SaveToFile(FILE* _File)
{
}

void GOlderMan::LoadFromFile(FILE* _File)
{
}
