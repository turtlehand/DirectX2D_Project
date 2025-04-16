#include "pch.h"
#include "GPlayerUseItemState.h"

#include <Engine/GTimeManager.h>

#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include "GPlayer.h"
#include <Engine/components.h>

#include <Engine/GPrefab.h>
#include <Engine/GGameObject.h>

#include "GPlatform.h"
#include "GObjectBasic.h"

GPlayerUseItemState::GPlayerUseItemState()
	: GScript(PLAYERUSEITEMSTATE)
	, m_Player(nullptr)
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

}

void GPlayerUseItemState::Tick()
{
	if (m_Player->m_PlayerUseItem == PLAYER_ITEM::HOOK)
	{
		Tick_Hook();
	}
}

void GPlayerUseItemState::Exit()
{
	m_Player->m_PlayerUseItem = PLAYER_ITEM::END;
}

void GPlayerUseItemState::ChangeState()
{
}

void GPlayerUseItemState::Enter_Hug()
{
	// �տ� ������Ʈ�� �ִ��� Ȯ���Ѵ�.
	LAYER_TYPE Type[2] = { LAYER_TYPE::MONSTER,LAYER_TYPE::OBJCET };

	for (int i = 0; i < size(Type); ++i)
	{
		const vector<GGameObject*>& vecObject = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)Type[i])->GetObjects();

		GObjectBasic* FrontOB = nullptr;

		for (int i = 0; i < vecObject.size(); ++i)
		{
			GObjectBasic* pOB = vecObject[i]->GetComponent<GObjectBasic>();
			assert(pOB);

			Vector3 OBPos = pOB->Transform()->GetWorldPos();
			Vector3 OBScale = pOB->Transform()->GetWorldScale();

			// �÷��̾ �ٶ󺸰� �ִ� ���⿡ ���ٸ� �Ѿ��.
			// �Ǵ� ������Ʈ�� �Ÿ��� �ʹ� �ִٸ� �Ѿ��.
			if (PFPos.y <= Transform()->GetWorldPos().y)
				continue;

			// ������Ʈ�� ũ�� �ȿ� �÷��̾ ���ٸ� �Ѿ��.
			if (Transform()->GetWorldPos().x < PFPos.x - PFScale.x / 2 || PFPos.x + PFScale.x / 2 < Transform()->GetWorldPos().x)
				continue;

			// ù ����̶�� �����Ѵ�.
			// ����� �÷��̾�� �� �����ٸ� �����Ѵ�.
			if (AbovePF == nullptr || PFPos.y < AbovePF->Transform()->GetWorldPos().y)
			{
				AbovePF = pPlatform;
			}
		}
	}




	// �տ� ������Ʈ�� �ִٸ� ���� ������.

	// ���ٸ� ��� ��� ���Ѵ�.
}

void GPlayerUseItemState::Tick_Hug()
{
}

void GPlayerUseItemState::Exit_Hug()
{
}

void GPlayerUseItemState::Tick_Hook()
{
	m_PlayerRigid->AddForce(Vector2(0.f, m_PlayerRigid->GetFriction())* 2 * DT);

	// �ӷ��� m_MaxMoveSpeed���� Ŀ���� ����
	if (m_Player->m_HookMaxSpeed <= m_PlayerRigid->GetVelocity().y)
	{
		Vector2 PlayerSpeed = m_PlayerRigid->GetVelocity();
		PlayerSpeed.y = m_Player->m_HookMaxSpeed;
		m_PlayerRigid->SetVelocity(PlayerSpeed);
	}
	
}

void GPlayerUseItemState::OnTriggerEnter(GCollider2D* _Other)
{
	// Hook ��� ���¿��� Wood Platform�� ��Ҵٸ� ����

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