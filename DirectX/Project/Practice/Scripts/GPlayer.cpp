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

#include "GPlayerDefaultState.h"
#include "GPlayerWalkState.h"
#include "GPlayerUseItemState.h"
#include "GPlayerJumpState.h"
#include "GPlayerFallState.h"

#include "GPlatform.h"

GPlayer::GPlayer()
	: GObjectBasic(PLAYER)
	, m_FSM(nullptr)
	, m_MoveInitForce(100.f)
	, m_MoveMaxSpeed(10.f)

	, m_HookInitForce(30.f)
	, m_HookMaxSpeed(30.f)

	, m_GravityScale(200)

	,m_JumpTimeLimit(1.f)
	,m_JumpTimer(0.0f)
	,m_JumpPower(20.f)
{

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
}

void GPlayer::Begin()
{
	m_FSM = GameObject()->GetComponent<GFSM>();

	m_FSM->AddState(L"Default", new GPlayerDefaultState);
	m_FSM->AddState(L"Walk", new GPlayerWalkState);
	m_FSM->AddState(L"UseItem", new GPlayerUseItemState);
	m_FSM->AddState(L"Jump", new GPlayerJumpState);
	m_FSM->AddState(L"Fall", new GPlayerFallState);

	m_FSM->ChanageState(L"Default");
}

void GPlayer::Update()
{
	KeyInput();
}

void GPlayer::OnTriggerEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
		return;

	Vector3 OtherPos = _Other->Transform()->GetWorldPos();
	Vector3 OtherScale = _Other->Transform()->GetWorldScale();
	Vector3 ThisPos = GameObject()->Transform()->GetWorldPos();
	Vector3 ThisScale = GameObject()->Transform()->GetWorldScale();

	// 닿은 대상이 플레이어보다 아래에 있다면
	if (OtherPos.y + OtherScale.y < ThisPos.y - ThisScale.y)
	{
		m_FSM->ChanageState(L"Default");
	}
}

void GPlayer::OnTriggerStay(GCollider2D* _Other)
{
}

void GPlayer::OnTriggerExit(GCollider2D* _Other)
{

}

void GPlayer::SaveToFile(FILE* _File)
{

}

void GPlayer::LoadFromFile(FILE* _File)
{

}

void GPlayer::SetMoveDirection(int _Direction)
{
	if (_Direction == 0)
		return;
	Vector3 DefaultScale = Transform()->GetRelativeScale();
	DefaultScale.x = _Direction * fabs(DefaultScale.x);
	Transform()->SetRelativeScale(DefaultScale);
}

void GPlayer::KeyInput()
{

	m_KeyInput = tKeyInput();

	// 키입력에 따라서 사각형이 움직이도록 한다.
	m_KeyInput.HorizontalMove = 0;
	m_KeyInput.HorizontalMove -= KEY_PRESSED(KEY::LEFT) ? 1 : 0;
	m_KeyInput.HorizontalMove += KEY_PRESSED(KEY::RIGHT) ? 1 : 0;


	m_KeyInput.Jump = KEY_PRESSED(KEY::Z);
	m_KeyInput.Interaction = KEY_DOWN(KEY::X);

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

	// 현재 아이템을 쓸만한 상황인지 확인한다.
	// ItemCheck
	if (ItemCheck())
	{
		m_FSM->ChanageState(L"UseItem");
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

// 아이템을 쓸만한 상황인지 확인한다.
// 쓸만한 상황이라면 m_PlayerUseItem = true가 되며
// ItemCheck는 True를 반환한다.
bool GPlayer::ItemCheck()
{
	if (Hook())
	{
		m_PlayerUseItem = PLAYER_ITEM::HOOK;
		return true;
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
		
		// 플레이어가 해당 플렛폼 안에 없다면 넘어간다.
		if (Transform()->GetWorldPos().x < PFPos.x - PFScale.x / 2 || PFPos.x + PFScale.x / 2 <  Transform()->GetWorldPos().x)
			continue;

		if (AbovePF == nullptr || PFPos.y < AbovePF->Transform()->GetWorldPos().y)
		{
			AbovePF = pPlatform;
		}
	}

	if (AbovePF == nullptr)
		return false;

	// 나무 플랫폼이 아니라면 없음
	if (AbovePF->GetPlatformType() != PLATFORM_TYPE::WOOD)
		return false;

	return true;
}

bool GPlayer::Bomb()
{
	// 해당 아이템을 갖고 있지 않다면 false를 반환
	if (!m_PlayerItems[(int)PLAYER_ITEM::BOMB])
		return false;

	// 근처에 바위나 금간
}

bool GPlayer::LandCheck()
{



	return false;
}

bool GPlayer::FallCheck()
{
	return false;
}