#include "pch.h"
#include "GPrincess.h"

#include <Engine/GTimeManager.h>

#include <Engine/components.h>
#include <Engine/GAssetManager.h>

#include "GGameManager.h"

GPrincess::GPrincess()
	: GObjectBasic(PRINCESS)
{
}

GPrincess::~GPrincess()
{
}

void GPrincess::Init()
{
}

void GPrincess::Begin()
{
}

void GPrincess::Update()
{
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
			Transform()->SetRelativeRotation(0.f, 0.f, 90.f * (1 - ratio) + 75.f * ratio);
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

void GPrincess::OnOverlapEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER_ATTACK)
	{
		// 피 파티클 생성
		GGameObject* pBlood = GAssetManager::GetInst()->FindAsset<GPrefab>(L"Prefab\\Blood.prefab")->Instantiate();
		SpawnGameObject(pBlood);
		GameObject()->SetChild(pBlood);

		// 죽음
		if (!m_IsDead)
		{
			m_IsDead = true;
			m_Timer = 0.f;

			// 엔딩
			GGameManager::GetInst()->GameEnding(ENDING_TYPE::Stress_Relief);
		}
	}
}

void GPrincess::SaveToFile(FILE* _File)
{
}

void GPrincess::LoadFromFile(FILE* _File)
{
}


