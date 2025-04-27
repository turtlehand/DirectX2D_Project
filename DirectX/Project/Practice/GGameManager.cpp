#include "pch.h"
#include "GGameManager.h"

#include <Engine/GAssetManager.h>
#include <Engine/GSprite.h>
#include <Engine/GPrefab.h>

#include <Engine/GTimeManager.h>
#include <Engine/GRenderManager.h>
#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include <Engine/GKeyManager.h>

#include <Engine/components.h>

#include "GEndingCamera.h"
#include "GEndingScene.h"

extern const vector<string> EndingName = {
	"Older_Man_Attack",			// ���� ����
	"Mighty_Minion",			// �̴Ͼ𿡰� ���
	"Cruel_Minion_Killer",		// �̴Ͼ� ������ ���
	"Mundane_Pouch",			// ������ ���� �� �ٸ� �η�����
	"Disappointing_Hero",		// ���θ��� �Ѿ���
	"Delicious_Strawberry",		// �� ����� ����
	"Battleground",				// ���й�Ʋ�׶���
	"Yahoo",					// ������ ���
	"Lava",						// ���
	"Taking_Day_Off",			// ������
	"Stress_Relief",			// ���ֿ��� ��Ǯ��
	"Execution",				// ���տ��� ó��
	"Cowardly_Hero",			// ���� ��ġ
	"Bear_Hug",					// �㸮�� �η��� ����
	"Minon_Allergy",			// �̴Ͼ� �˷�����
	"Rejection",				// ��¥
	"LovePower",				// ����� ��
	"Leap_of_Faith",			// �ŷ��� ����
	"Abyss",
	"Home",
	"END"
};

GGameManager::GGameManager()
	: m_EndingTime(30.f)
	, m_PlayType(PLAY_TYPE::END)
{

}

GGameManager::~GGameManager()
{

}

void GGameManager::CallDarkLord()
{
	if (m_DarkLord == nullptr)
	{
		return;
	}
	SpawnGameObject(m_DarkLord->Instantiate());
}

void GGameManager::Init()
{
	m_EndingScene[0] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Older_Man_Attack.sprite");		// ���� ����
	m_EndingScene[1] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Mighty_Minion.sprite");			// �̴Ͼ𿡰� ���
	m_EndingScene[2] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Cruel_Minion_Killer.sprite");	// �̴Ͼ� ������ ���	
	m_EndingScene[3] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Mundane_Pouch.sprite");			// ������ ���� �� �ٸ� �η�����
	m_EndingScene[4] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Disappointing_Hero.sprite");		// ���θ��� �Ѿ���
	m_EndingScene[5] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Delicious_Strawberry.sprite");	// �� ����� ����	
	m_EndingScene[6] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Battleground.sprite");			// ���й�Ʋ�׶���
	m_EndingScene[7] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Yahoo.sprite");					// ������ ���
	m_EndingScene[8] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Lava.sprite");					// ���
	m_EndingScene[9] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Taking_Day_Off.sprite");			// ������
	m_EndingScene[10] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Stress_Relief.sprite");			// ���ֿ��� ��Ǯ��
	m_EndingScene[11] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Execution.sprite");				// ���տ��� ó��
	m_EndingScene[12] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Cowardly_Hero.sprite");			// ���� ��ġ
	m_EndingScene[13] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Bear_Hug.sprite");				// �㸮�� �η��� ����
	m_EndingScene[14] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Minon_Allergy.sprite");			// �̴Ͼ� �˷�����
	m_EndingScene[15] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Rejection.sprite");				// ��¥
	m_EndingScene[16] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\LovePower.sprite");				// ����� ��
	m_EndingScene[17] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Leap_of_Faith.sprite");			// �ŷ��� ����	
	m_EndingScene[18] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Abyss.sprite");
	m_EndingScene[19] = GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\Ending\\Home.sprite");

}

void GGameManager::Begin()
{
	m_PlayType = PLAY_TYPE::PLAY;
	GGameObject* pCamera = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(31)->FindObject(L"EndingCamera");
	GGameObject* pScene = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(31)->FindObject(L"EndingScene");

	m_Camera = pCamera->GetComponent<GEndingCamera>();
	assert(m_Camera);

	m_Scene = pScene->GetComponent<GEndingScene>();
	assert(m_Scene);

	GameLoad();
}

void GGameManager::Progress()
{
	if (GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::PLAY)
		return;

	if (m_PlayType == PLAY_TYPE::END)
		return;

	if (m_PlayType == PLAY_TYPE::ENDING_SCENE)
	{
		if (m_EndingTime < m_EndingTimer)
		{
			//GTimeManager::GetInst()->SetTimeScale(1.f);

			//GRenderManager::GetInst()->DeRegisterCamera(m_Camera->Camera());

			m_PlayType = PLAY_TYPE::RETRY;
		}
		else
		{
			if (m_EndingTimer < 3.f)
			{
				// ��ġ	0.25 ~ 0
				// ��	0    ~ 3
				float ratio = (3 - m_EndingTimer) / 12; 
				GTimeManager::GetInst()->SetTimeScale(ratio);
				m_Scene->SpriteRender()->SetColor(Vector4(1.f, 1.f, 1.f, 0.f));
			}
			else if (3.f < m_EndingTimer && m_EndingTimer < 6.f)
			{
				GTimeManager::GetInst()->SetTimeScale(0);
				// ��ġ	0 ~ 1
				// ��	3 ~ 10
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
	else if (m_PlayType == PLAY_TYPE::RETRY)
	{
		if (KEY_DOWN(KEY::X))
		{

		}
	}
}

void GGameManager::End()
{
	m_PlayType = PLAY_TYPE::END;
}

void GGameManager::GameLoad()
{
	GTimeManager::GetInst()->SetTimeScale(1.f);
	GRenderManager::GetInst()->DeRegisterCamera(m_Camera->Camera());
	m_PlayType = PLAY_TYPE::PLAY;
	m_EndingTimer = 0.f;
}

void GGameManager::GameEnding(ENDING_TYPE _Type)
{
	if (m_PlayType == PLAY_TYPE::ENDING_SCENE || m_PlayType == PLAY_TYPE::END || _Type == ENDING_TYPE::END)
		return;

	m_PlayType = PLAY_TYPE::ENDING_SCENE;
	m_EndingTimer = 0.f;
	
	m_Scene->SpriteRender()->SetSprite(m_EndingScene[(UINT)_Type]);
	m_Scene->SpriteRender()->SetColor(Vector4(1.f, 1.f, 1.f, 0.f));
	GRenderManager::GetInst()->RegisterCamera(m_Camera->Camera(), 1);
}
