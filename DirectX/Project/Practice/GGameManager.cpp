#include "pch.h"
#include "GGameManager.h"

#include <Engine/GTimeManager.h>
#include <Engine/GRenderManager.h>
#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include <Engine/components.h>

#include "GEndingCamera.h"
#include "GEndingScene.h"

GGameManager::GGameManager()
	:m_EndingTime(10.f)
	, m_IsEnd(false)
{

}

GGameManager::~GGameManager()
{

}

void GGameManager::Init()
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
			m_EndingTimer += ENGINEDT;
		}
	}

}

void GGameManager::GameEnding(ENDING_TYPE _Type)
{
	m_IsEnd = true;
	m_EndingTimer = 0.f;

	GTimeManager::GetInst()->SetTimeScale(0.f);

	GRenderManager::GetInst()->RegisterCamera(m_Camera->Camera(), 1);
}
