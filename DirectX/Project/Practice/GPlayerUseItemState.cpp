#include "pch.h"
#include "GPlayerUseItemState.h"

#include <Engine/GTimeManager.h>
#include <Engine/GRenderManager.h>

#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include "GPlayer.h"
#include <Engine/components.h>

#include <Engine/GPrefab.h>
#include <Engine/GGameObject.h>

#include "GPlatform.h"
#include "GObjectBasic.h"

#include "GOlderMan.h"
#include "GMinion.h"
#include "GPrincess.h"

GPlayerUseItemState::GPlayerUseItemState()
	: m_Player(nullptr)
	, m_PlayerRigid(nullptr)
{

}

GPlayerUseItemState::~GPlayerUseItemState()
{

}

void GPlayerUseItemState::Awake()
{
	m_Player = GetFSM()->GameObject()->GetComponent<GPlayer>();
	m_PlayerRigid = m_Player->RigidBody2D();

}

void GPlayerUseItemState::Enter()
{
	if (!m_Player->FlipbookRender())
		return;

	//m_Player->FlipbookRender()->Play((int)PLAYER_STATE::DEFAULT);

	m_Player->m_PlayerState = PLAYER_STATE::USEITEM;

	if (m_Player->m_PlayerUseItem == PLAYER_ITEM::HUG)
	{
		Enter_Hug();
	}
	else if (m_Player->m_PlayerUseItem == PLAYER_ITEM::SHOVEL)
	{
		Enter_Shovel();
	}

}

void GPlayerUseItemState::Tick()
{
	if (m_Player->m_PlayerUseItem == PLAYER_ITEM::HOOK)
	{
		Tick_Hook();
	}
	else if (m_Player->m_PlayerUseItem == PLAYER_ITEM::SHOVEL)
	{
		Tick_Shovel();
	}
	else if (m_Player->m_PlayerUseItem == PLAYER_ITEM::HUG)
	{
		Tick_Hug();
	}
}

void GPlayerUseItemState::Exit()
{
	if (m_Player->m_PlayerUseItem == PLAYER_ITEM::SHOVEL)
	{
		Exit_Shovel();
	}

	m_Player->m_PlayerUseItem = PLAYER_ITEM::END;
}

void GPlayerUseItemState::ChangeState()
{
}

void GPlayerUseItemState::Enter_Hug()
{
	// 앞에 오브젝트가 있는지 확인한다.
	LAYER_TYPE Type[3] = { LAYER_TYPE::MONSTER, LAYER_TYPE::NPC, LAYER_TYPE::ETC_OBJCET };
	GObjectBasic* FrontOB = nullptr;
	Vector3 ThisPos = m_Player->Transform()->GetWorldPos();

	//DrawDebugRect(Vector4(1.f, 1.f, 0.f, 1.f), ThisPos, Vector3(m_Player->m_HugDetectScale.x, m_Player->m_HugDetectScale.y, -1.f), Vector3(0.f, 0.f, 0.f));
	
	for (int i = 0; i < size(Type); ++i)
	{
		const vector<GGameObject*>& vecObject = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)Type[i])->GetObjects();

		for (int i = 0; i < vecObject.size(); ++i)
		{
			GObjectBasic* pOB = vecObject[i]->GetComponent<GObjectBasic>();
			if (pOB == nullptr)
				continue;

			Vector3 OBPos = pOB->Transform()->GetWorldPos();
			Vector3 OBScale = pOB->Transform()->GetWorldScale();

			// 플레이어가 바라보고 있는 방향에 없다면 넘어간다.
			// 또는 오브젝트와 거리가 너무 멀다면 넘어간다.
			if (m_Player->GetDirection() == 1)
			{
				if (OBPos.x < ThisPos.x || m_Player->Transform()->GetWorldPos().x + ThisPos.x < OBPos.x)
					continue;
			}
			else
			{
				if (OBPos.x < ThisPos.x - m_Player->m_HugDetectScale.x || ThisPos.x  < OBPos.x)
					continue;
			}
			

			// 오브젝트의 크기 안에 플레이어가 없다면 넘어간다.
			if (OBPos.y < ThisPos.y - m_Player->m_HugDetectScale.y / 2
				|| ThisPos.y + m_Player->m_HugDetectScale.y / 2 < OBPos.y)
				continue;

			// 첫 대상이라면 설정한다.
			// 대상이 플레이어보다 더 가깝다면 설정한다.

			if (m_Player->GetDirection() == 1)
			{
				if (FrontOB == nullptr || OBPos.x < FrontOB->Transform()->GetWorldPos().x)
				{
					FrontOB = pOB;
				}
			}
			else
			{
				if (FrontOB == nullptr || FrontOB->Transform()->GetWorldPos().x < OBPos.x )
				{
					FrontOB = pOB;
				}
			}

		}
	}

	m_Player->m_ItemTimer = 0.f;

	// 앞에 오브젝트가 있다면 몸을 던진다.
	if (FrontOB)
	{
		m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::HUG);
		m_PlayerRigid->AddForce(Vector2(m_Player->GetDirection() * 60, 40));

		if (dynamic_cast<GOlderMan*>(FrontOB))
		{
			GGameManager::GetInst()->GameEnding(ENDING_TYPE::Bear_Hug);
		}
		else if (dynamic_cast<GMinion*>(FrontOB))
		{
			GGameManager::GetInst()->GameEnding(ENDING_TYPE::Minon_Allergy);
		}

	}

	// 없다면 허그 포즈만 취한다.
	else
	{
		m_Player->FlipbookRender()->Play((int)PLAYER_FLIPBOOK::HUG_READY);
	}
}

void GPlayerUseItemState::Tick_Hug()
{
	if (m_Player->m_ItemTimer > m_Player->m_HugTime)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
	}
	m_Player->m_ItemTimer += DT;
}

void GPlayerUseItemState::Exit_Hug()
{
}

void GPlayerUseItemState::Enter_Shovel()
{
	// 검 생성
	m_Player->m_Shovel = m_Player->m_ShovelPrefab->Instantiate();
	m_Player->m_ItemTimer = 0;

	SpawnGameObject(m_Player->m_Shovel);
	m_Player->GameObject()->SetChild(m_Player->m_Shovel);
}

void GPlayerUseItemState::Tick_Shovel()
{
	// 시간을 완료시 삭제
	if (m_Player->m_ItemTimer > m_Player->m_ShovelTime)
	{
		m_Player->GetFSM()->ChanageState(L"Default");
	}
	m_Player->m_ItemTimer += DT;
}

void GPlayerUseItemState::Exit_Shovel()
{
	m_Player->m_Shovel->Destroy();

	m_Player->m_DestroyPlatform->GameObject()->Destroy();
}

void GPlayerUseItemState::Tick_Hook()
{
	m_PlayerRigid->AddForce(Vector2(0.f, m_PlayerRigid->GetFriction())* 2 * DT);

	// 속력이 m_MaxMoveSpeed보다 커지면 감속
	if (m_Player->m_HookMaxSpeed <= m_PlayerRigid->GetVelocity().y)
	{
		Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();
		PlayerSpeed.y = m_Player->m_HookMaxSpeed;
		m_PlayerRigid->SetVelocity(PlayerSpeed);
	}
	
}

void GPlayerUseItemState::OnOverlapEnter(GCollider2D* _Other)
{
	// Hook 사용 상태에서 Wood Platform에 닿았다면 변경

	if (m_Player->m_PlayerUseItem == PLAYER_ITEM::HOOK)
	{
		if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
			return;

		GPlatform* pPlatform = _Other->GameObject()->GetComponent<GPlatform>();
		assert(pPlatform);

		Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();
		PlayerSpeed.y = 0;
		m_PlayerRigid->SetVelocity(PlayerSpeed);

		GetFSM()->ChanageState(L"Default");
	}
	


}


void GPlayerUseItemState::SaveToFile(FILE* _File)
{
}

void GPlayerUseItemState::LoadFromFile(FILE* _File)
{
}