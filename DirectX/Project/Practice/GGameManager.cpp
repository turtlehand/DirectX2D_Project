#include "pch.h"
#include "GGameManager.h"

#include <Engine/GAssetManager.h>
#include <Engine/GSprite.h>

#include <Engine/GTimeManager.h>
#include <Engine/GRenderManager.h>
#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include <Engine/components.h>

#include "GEndingCamera.h"
#include "GEndingScene.h"

GGameManager::GGameManager()
	: m_EndingTime(30.f)
	, m_IsEnd(false)
{

}

GGameManager::~GGameManager()
{

}

void GGameManager::Init()
{
	m_EndingScene[0] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Older_Man_Attack.sprite");		// 노인 공격
	m_EndingScene[1] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Mighty_Minion.sprite");			// 미니언에게 사망
	m_EndingScene[2] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Cruel_Minion_Killer.sprite");	// 미니언 여러번 찌르기	
	m_EndingScene[3] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Mundane_Pouch.sprite");			// 아이템 많이 들어서 다리 부러지기
	m_EndingScene[4] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Disappointing_Hero.sprite");		// 돌부리에 넘어짐
	m_EndingScene[5] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Delicious_Strawberry.sprite");	// 산 꼭대기 딸기	
	m_EndingScene[6] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Battleground.sprite");			// 갑분배틀그라운드
	m_EndingScene[7] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Yahoo.sprite");					// 마리오 토관
	m_EndingScene[8] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Lava.sprite");					// 용암
	m_EndingScene[9] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Taking_Day_Off.sprite");			// 집돌이
	m_EndingScene[10] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Stress_Relief.sprite");			// 공주에게 분풀이
	m_EndingScene[11] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Execution.sprite");				// 마왕에게 처형
	m_EndingScene[12] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Cowardly_Hero.sprite");			// 마왕 뒤치
	m_EndingScene[13] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Bear_Hug.sprite");				// 허리가 부러진 노인
	m_EndingScene[14] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Minon_Allergy.sprite");			// 미니언 알레르기
	m_EndingScene[15] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Rejection.sprite");				// 퇴짜
	m_EndingScene[16] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\LovePower.sprite");				// 사랑의 힘
	m_EndingScene[17] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Leap_of_Faith.sprite");			// 신뢰의 도약	


}

void GGameManager::Begin()
{
	GGameObject* pCamera = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(31)->FindObject(L"EndingCamera");
	GGameObject* pScene = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(31)->FindObject(L"EndingScene");

	m_Camera = pCamera->GetComponent<GEndingCamera>();
	assert(m_Camera);

	m_Scene = pScene->GetComponent<GEndingScene>();
	assert(m_Scene);
}

void GGameManager::Progress()
{
	if (GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::PLAY)
	{
		GTimeManager::GetInst()->SetTimeScale(1.f);
		return;
	}

	if (m_IsEnd)
	{
		if (m_EndingTime < m_EndingTimer)
		{
			GTimeManager::GetInst()->SetTimeScale(1.f);

			GRenderManager::GetInst()->DeRegisterCamera(m_Camera->Camera());

			m_IsEnd = false;
		}
		else
		{
			if (m_EndingTimer < 3.f)
			{
				// 수치	0.25 ~ 0
				// 초	0    ~ 3
				float ratio = (3 - m_EndingTimer) / 12; 
				GTimeManager::GetInst()->SetTimeScale(ratio);
				m_Scene->SpriteRender()->SetColor(Vector4(1.f, 1.f, 1.f, 0.f));
			}
			else if (3.f < m_EndingTimer && m_EndingTimer < 6.f)
			{
				GTimeManager::GetInst()->SetTimeScale(0);
				// 수치	0 ~ 1
				// 초	3 ~ 10
				float ratio = (6 - m_EndingTimer) / 3;
				//GTimeManager::GetInst()->SetTimeScale(ratio);
				m_Scene->SpriteRender()->SetColor(Vector4(1.f, 1.f, 1.f, 1 - ratio));
			}
			else
			{
				GTimeManager::GetInst()->SetTimeScale(0);
				m_Scene->SpriteRender()->SetColor(Vector4(1.f, 1.f, 1.f, 1.f));
			}
			m_EndingTimer += ENGINEDT;
		}
	}

}

void GGameManager::GameLoad()
{
}

void GGameManager::GameEnding(ENDING_TYPE _Type)
{
	if (m_IsEnd)
		return;

	m_IsEnd = true;
	m_EndingTimer = 0.f;

	m_Scene->SpriteRender()->SetSprite(m_EndingScene[(UINT)_Type]);

	GRenderManager::GetInst()->RegisterCamera(m_Camera->Camera(), 1);
}
