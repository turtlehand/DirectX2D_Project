#include "pch.h"
#include "GPlayer.h"
#include <Engine/GGameObject.h>
#include <Engine/components.h>
#include <Engine/GTimeManager.h>
#include <Engine/GKeyManager.h>
#include <Engine/GRenderManager.h>
#include <Engine/GAssetManager.h>
#include <Engine/GTaskManager.h>
#include <Engine/GLevelManager.h>
#include <Engine/GLayer.h>
#include <Engine/GLevel.h>



#include "GFSM.h"
#include "GItem.h"

#include "GPlayerDefaultState.h"
#include "GPlayerWalkState.h"
#include "GPlayerUseItemState.h"
#include "GPlayerJumpState.h"
#include "GPlayerFallState.h"
#include "GPlayerFlinchState.h"
#include "GPlayerGetItemState.h"

#include "GPlayerNoneState.h"

#include "GPlatform.h"
#include "GPrincess.h"

GPlayer::GPlayer()
	: GObjectBasic(PLAYER)
	, m_KeyInput()
	, m_FSM(nullptr)

	, m_PlayerState(PLAYER_STATE::DEFAULT)

	, m_MoveInitForce(100.f)
	, m_MoveMaxSpeed(20.f)

	, m_JumpTimeLimit(0.4f)
	, m_JumpTimeMin(0.05f)
	, m_JumpTimer(0.0f)
	, m_JumpPower(400.f)
	, m_JumpMaxSpeed(50.f)

	, m_PlayerItems{ false }	// 현재 가지고 있는 아이템
	, m_PlayerUseItem(PLAYER_ITEM::END)				// 현재 사용중인 아이템 END라면 사용 중 X
	, m_ItemMaxCount(4)

	, m_ItemTimer(0.f)

	, m_HookInitForce(30.f)
	, m_HookMaxSpeed(30.f)

	, m_Bomb(nullptr)
	, m_BombPrefab(nullptr)
	, m_BombDetectScale()
	, m_BombTime(0.1f)

	, m_Shovel(nullptr)
	, m_ShovelPrefab(nullptr)
	, m_DestroyPlatform(nullptr)
	, m_ShovelTime(0.5f)

	, m_HugDetectScale(Vector2(20,10))
	, m_HugTime(0.5f)

	, m_Sword(nullptr)
	, m_SwordPrefab(nullptr)
	, m_SwordPos()
	, m_SwordTime(0.3f)
{
	for (int i = 0; i < (int)PLAYER_ITEM::END; ++i)
	{
		m_PlayerItems[i] = false;
	}
}

GPlayer::GPlayer(const GPlayer& _Origin)
	: GObjectBasic(PLAYER)
	, m_KeyInput()
	, m_FSM(nullptr)

	, m_PlayerState(PLAYER_STATE::DEFAULT)

	, m_MoveInitForce(_Origin.m_MoveInitForce)
	, m_MoveMaxSpeed(_Origin.m_MoveMaxSpeed)

	, m_JumpTimeLimit(_Origin.m_JumpTimeLimit)
	, m_JumpTimeMin(_Origin.m_JumpTimeMin)
	, m_JumpTimer(0.0f)
	, m_JumpPower(_Origin.m_JumpPower)
	, m_JumpMaxSpeed(_Origin.m_JumpMaxSpeed)

	, m_PlayerItems{false}	// 현재 가지고 있는 아이템
	, m_PlayerUseItem(PLAYER_ITEM::END)				// 현재 사용중인 아이템 END라면 사용 중 X
	, m_ItemMaxCount(4)

	, m_ItemTimer(0.f)

	, m_HookInitForce(_Origin.m_HookInitForce)
	, m_HookMaxSpeed(_Origin.m_HookMaxSpeed)

	, m_Bomb(nullptr)
	, m_BombPrefab(_Origin.m_BombPrefab)
	, m_BombDetectScale(_Origin.m_BombDetectScale)
	, m_BombTime(_Origin.m_BombTime)

	, m_Shovel(nullptr)
	, m_ShovelPrefab(_Origin.m_ShovelPrefab)
	, m_DestroyPlatform(nullptr)
	, m_ShovelTime(_Origin.m_ShovelTime)

	, m_HugDetectScale(_Origin.m_HugDetectScale)
	, m_HugTime(_Origin.m_HugTime)

	, m_Sword(nullptr)
	, m_SwordPrefab(_Origin.m_SwordPrefab)
	, m_SwordPos(_Origin.m_SwordPos)
	, m_SwordTime(_Origin.m_SwordTime)
{
	for (int i = 0; i <(int) PLAYER_ITEM::END; ++i)
	{
		m_PlayerItems[i] = _Origin.m_PlayerItems[i];
	}
}

GPlayer::~GPlayer()
{
}

void GPlayer::Init()
{
	ADD_FLOAT("InitMoveForce", &m_MoveInitForce);
	ADD_FLOAT("MaxMoveSpeed", &m_MoveMaxSpeed);

	ADD_FLOAT("JumpTimeLimit", &m_JumpTimeLimit);
	ADD_FLOAT("JumpTimer", &m_JumpTimer);
	ADD_FLOAT("JumpPower", &m_JumpPower);

	ADD_BOOL("HOOK", &m_PlayerItems[(INT)PLAYER_ITEM::HOOK]);
	ADD_FLOAT("HookInitForce", &m_HookInitForce);
	ADD_FLOAT("HookMaxSpeed", &m_HookMaxSpeed);

	ADD_BOOL("SWORD", &m_PlayerItems[(INT)PLAYER_ITEM::SWORD]);
	ADD_PREFAB("SwordPrefab", &m_SwordPrefab);
	ADD_VECTOR3("SwrodPos", &m_SwordPos)
	ADD_FLOAT("SwrodTime", &m_SwordTime);

	ADD_BOOL("SHOVEL", &m_PlayerItems[(INT)PLAYER_ITEM::SHOVEL]);
	ADD_PREFAB("ShovelPrefab", &m_ShovelPrefab);
	ADD_FLOAT("ShovelTime", &m_ShovelTime);

	ADD_BOOL("HUG", &m_PlayerItems[(INT)PLAYER_ITEM::HUG]);
	ADD_VECTOR2("HugDetectScale", &m_HugDetectScale);
	ADD_FLOAT("HugTime", &m_HugTime);

	ADD_BOOL("PRINCESS", &m_PlayerItems[(INT)PLAYER_ITEM::PRINCESS]);

}

void GPlayer::Begin()
{
	m_MaxHP = 3;
	m_HP = 3;

	m_FSM = GameObject()->GetComponent<GFSM>();

	m_FSM->AddState(L"Default", new GPlayerDefaultState);
	m_FSM->AddState(L"Walk", new GPlayerWalkState);
	m_FSM->AddState(L"UseItem", new GPlayerUseItemState);
	m_FSM->AddState(L"Jump", new GPlayerJumpState);
	m_FSM->AddState(L"Fall", new GPlayerFallState);
	m_FSM->AddState(L"Flinch", new GPlayerFlinchState);
	m_FSM->AddState(L"GetItem", new GPlayerGetItemState);

	m_FSM->AddState(L"None", new GPlayerNoneState);

	m_FSM->ChanageState(L"Default");

	m_SwordPrefab = GAssetManager::GetInst()->FindAsset<GPrefab>(L"Prefab\\Sword_Player.prefab");
	m_ShovelPrefab = GAssetManager::GetInst()->FindAsset<GPrefab>(L"Prefab\\Shovel_Player.prefab");

	m_SwordSound = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\Sword.wav", L"Sound\\AudioClip\\Sword.wav");
}

void GPlayer::Update()
{
	GObjectBasic::Update();
	KeyInput();
	UseItem();
}

void GPlayer::OnOverlapEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::ENEMY_ATTACK && m_PlayerState != PLAYER_STATE::FLINCH)
	{
		m_FSM->ChanageState(L"Flinch");
		int Dir = Transform()->GetWorldPos().x - _Other->Transform()->GetWorldPos().x;
		Dir = Dir / (Dir == 0 ? 1 : abs(Dir));
		RigidBody2D()->AddForce(Vector2(Dir * m_FlinchForce.x, m_FlinchForce.y));
	}
	
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::ITEM && (m_PlayerState == PLAYER_STATE::DEFAULT || m_PlayerState == PLAYER_STATE::WALK))
	{
		GGameObject* Object = _Other->GameObject();
		assert(Object);

		m_FSM->ChanageState(L"GetItem",(DWORD_PTR)Object);
	}

	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::NPC && (m_PlayerState == PLAYER_STATE::DEFAULT || m_PlayerState == PLAYER_STATE::WALK))
	{
		GGameObject* Object = _Other->GameObject();
		assert(Object);

		if (_Other->GameObject()->GetComponent<GPrincess>())
		{
			m_FSM->ChanageState(L"GetItem", (DWORD_PTR)Object);
		}
		
	}

	//if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
	//	return;

	//Vector3 OtherPos = _Other->Transform()->GetWorldPos();
	//Vector3 OtherScale = _Other->Transform()->GetWorldScale();
	//Vector3 ThisPos = GameObject()->Transform()->GetWorldPos();
	//Vector3 ThisScale = GameObject()->Transform()->GetWorldScale();

	// 닿은 대상이 플레이어보다 아래에 있다면
	//if (OtherPos.y + OtherScale.y < ThisPos.y - ThisScale.y)
	//{
	//	m_FSM->ChanageState(L"Default");
	//}
}

void GPlayer::OnOverlapStay(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::ETC_OBJCET)
	{
		m_NearObject = _Other->GameObject();
	}
}

void GPlayer::OnOverlapExit(GCollider2D* _Other)
{
	m_NearObject = nullptr;
}

void GPlayer::SaveToFile(FILE* _File)
{

}

void GPlayer::LoadFromFile(FILE* _File)
{

}

void GPlayer::KeyInput()
{
	m_KeyInput = tKeyInput();

	// 키입력에 따라서 사각형이 움직이도록 한다.
	m_KeyInput.HorizontalMove = 0;
	m_KeyInput.HorizontalMove -= KEY_PRESSED(KEY::LEFT) ? 1 : 0;
	m_KeyInput.HorizontalMove += KEY_PRESSED(KEY::RIGHT) ? 1 : 0;

	m_KeyInput.Jump = KEY_DOWN(KEY::Z);
	m_KeyInput.JumpHold = KEY_PRESSED(KEY::Z);
	m_KeyInput.Interaction = KEY_DOWN(KEY::X);

}

// 검과 폭탄같이 다른 행동이 계속 가능한 대상은 Player에서 처리한다.
void GPlayer::UseItem()
{
	if (m_PlayerUseItem == PLAYER_ITEM::SWORD)
	{
		// 시간을 완료시 삭제
		if (m_ItemTimer > m_SwordTime)
		{
			m_PlayerUseItem = PLAYER_ITEM::END;
			m_Sword->Destroy();
		}
		m_ItemTimer += DT;
	}
}

// 상호작용을 할만한 대상이 있는지 확인하고
// 적절한 상화에 따라 State를 변경한다.
bool GPlayer::Interaction()
{
	// 근처에 상자가 있는지 확인한다.
	// 상자가 있다면 상자 상태로 변환다.
	if (BoxCheck())
	{
		//m_FSM->ChanageState(L"OpenBox");
		return true;
	}
	else if (BedCheck())
	{
		return true;
	}

	// 현재 아이템을 쓸만한 상황인지 확인한다.
	// ItemCheck
	if (ItemCheck())
	{
		return true;
	}
	return false;
}

// 근처에 상자 있는지 확인한다.
// 상자가 근처에 있다면 BoxCheck는 true를 반환한다.
bool GPlayer::BoxCheck()
{
	return false;
}

bool GPlayer::BedCheck()
{
	//if (m_NearObject->GetComponent<GBed>())
		//return true;

	return false;
}

// 아이템을 쓸만한 상황인지 확인한다.
// 쓸만한 상황이라면 m_PlayerUseItem = true가 되며
// ItemCheck는 True를 반환한다.

// 검과 폭탄을 제외한 아이템들은 상태로 넘어간다.
bool GPlayer::ItemCheck()
{
	if (m_PlayerUseItem != PLAYER_ITEM::END)
		return false;

	if (Hook())
	{
		m_PlayerUseItem = PLAYER_ITEM::HOOK;
		return true;
	}
	else if (Shovel())
	{
		m_PlayerUseItem = PLAYER_ITEM::SHOVEL;
		return true;
	}
	//else if (Bomb())
	//{
	//	m_PlayerUseItem = PLAYER_ITEM::BOMB;
	//	m_ItemTimer = 0;
	//}
	else if (Hug())
	{
		m_PlayerUseItem = PLAYER_ITEM::HUG;
		return true;
	}
	else if (Sword())
	{
		m_PlayerUseItem = PLAYER_ITEM::SWORD;

		// 검 생성
		m_Sword = m_SwordPrefab->Instantiate();
		m_ItemTimer = 0;

		SpawnGameObject(m_Sword);
		GameObject()->SetChild(m_Sword);

		m_SwordSound-> Play(1, GGameManager::GetInst()->GetEffect_Volume(), false);
	}


	return false;
}

bool GPlayer::Hook()
{
	// 해당 아이템을 갖고 있지 않다면 false를 반환
	if(!m_PlayerItems[(int)PLAYER_ITEM::HOOK])
		return false;

	// 플레이어 바로 위쪽에 나무가 있다면 true를 반환
	vector<GGameObject*> vecPlatform = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)LAYER_TYPE::PLATFORM)->GetObjects();

	GPlatform* AbovePF = nullptr;

	for (int i = 0; i < vecPlatform.size(); ++i)
	{
		GPlatform* pPlatform = vecPlatform[i]->GetComponent<GPlatform>();
		assert(pPlatform);
		
		Vector3 PFPos = pPlatform->Transform()->GetWorldPos();
		Vector3 PFScale = pPlatform->Transform()->GetWorldScale();
		
		// 이 게임은 어차피 회전하지 않으므로 단순하게 계산한다.

		// 해당 플랫폼이 플레이어 위에 없다면 넘어간다.
		if (PFPos.y <= Transform()->GetWorldPos().y)
			continue;

		if (pPlatform->GetPlatformType() == PLATFORM_TYPE::WOOD)
		{
			int a = 0;
		}
		
		// 플레이어가 해당 플렛폼 안에 없다면 넘어간다.
		Vector3 ThisPos = Transform()->GetWorldPos();
		if ((Transform()->GetWorldPos().x < PFPos.x - PFScale.x / 2) || (PFPos.x + PFScale.x / 2 <  Transform()->GetWorldPos().x))
			continue;

		if (AbovePF)
		{
			Vector3 AbovePos = AbovePF->Transform()->GetWorldPos();
			Vector3 AboveScale = AbovePF->Transform()->GetWorldScale();
		}
		
		if (AbovePF == nullptr || PFPos.y < AbovePF->Transform()->GetWorldPos().y)
		{
			AbovePF = pPlatform;
			int a = 0;

		}
	}

	if (AbovePF == nullptr)
		return false;

	// 나무 플랫폼이 아니라면 없음
	if (AbovePF->GetPlatformType() != PLATFORM_TYPE::WOOD)
		return false;

	return true;
}

/*
bool GPlayer::Bomb()
{
	// 해당 아이템을 갖고 있지 않다면 false를 반환
	if (!m_PlayerItems[(int)PLAYER_ITEM::BOMB])
		return false;

	GObjectBasic* OB = nullptr;

	const vector<GGameObject*>& vecObject = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)LAYER_TYPE::ETC_OBJCET)->GetObjects();

	for (int i = 0; i < vecObject.size(); ++i)
	{
		GObjectBasic* pOB = vecObject[i]->GetComponent<GObjectBasic>();
		assert(pOB);

		Vector3 OBPos = pOB->Transform()->GetWorldPos();
		Vector3 OBScale = pOB->Transform()->GetWorldScale();

		// 플레이어가 해당 플렛폼 안에 없다면 넘어간다.
			// 플레이어가 바라보고 있는 방향에 없다면 넘어간다.
			// 또는 오브젝트와 거리가 너무 멀다면 넘어간다.
		if (OBPos.x < Transform()->GetWorldPos().x - m_BombDetectScale.x || Transform()->GetWorldPos().x + m_BombDetectScale.x < OBPos.x)
			continue;

		if (OBPos.y < Transform()->GetWorldPos().y - m_BombDetectScale.y || Transform()->GetWorldPos().y + m_BombDetectScale.y < OBPos.y)
			continue;

		// 첫 대상이라면 설정한다.
		// 대상이 플레이어보다 더 가깝다면 설정한다.

		if (OB == nullptr)
		{
			OB = pOB;
			break;
		}

	}

	if (OB == nullptr)
		return false;

	return true;
}
*/
bool GPlayer::Shovel()
{
	if (!m_PlayerItems[(int)PLAYER_ITEM::SHOVEL])
		return false;

	if (!m_IsGround)
		return false;

	GPlatform* BottomPF = nullptr;

	for (int i = 0; i < m_vecPlatform.size(); ++i)
	{
		GPlatform* pPlatform = m_vecPlatform[i]->GetComponent<GPlatform>();
		assert(pPlatform);

		Vector3 PFPos = pPlatform->Transform()->GetWorldPos();
		Vector3 PFScale = pPlatform->Transform()->GetWorldScale();

		// 플레이어가 해당 플렛폼 안에 없다면 넘어간다.
		if (PFPos.x - PFScale.x / 2 <= Transform()->GetWorldPos().x && Transform()->GetWorldPos().x <= PFPos.x + PFScale.x / 2)
		{
			BottomPF = pPlatform;
		}

	}

	if (BottomPF == nullptr)
		return false;

	// 부드러운 땅일 때만
	if (BottomPF->GetPlatformType() != PLATFORM_TYPE::SOFT)
		return false;

	m_DestroyPlatform = BottomPF;
	return true;
}

bool GPlayer::Hug()
{
	// 해당 아이템을 갖고 있지 않다면 false를 반환
	if (!m_PlayerItems[(int)PLAYER_ITEM::HUG])
		return false;

	return true;
}

bool GPlayer::Sword()
{
	// 해당 아이템을 갖고 있지 않다면 false를 반환
	if (!m_PlayerItems[(int)PLAYER_ITEM::SWORD])
		return false;

	return true;
}