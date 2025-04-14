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

	// ���� ����� �÷��̾�� �Ʒ��� �ִٸ�
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

	// Ű�Է¿� ���� �簢���� �����̵��� �Ѵ�.
	m_KeyInput.HorizontalMove = 0;
	m_KeyInput.HorizontalMove -= KEY_PRESSED(KEY::LEFT) ? 1 : 0;
	m_KeyInput.HorizontalMove += KEY_PRESSED(KEY::RIGHT) ? 1 : 0;


	m_KeyInput.Jump = KEY_PRESSED(KEY::Z);
	m_KeyInput.Interaction = KEY_DOWN(KEY::X);

}

// ��ȣ�ۿ��� �Ҹ��� ����� �ִ��� Ȯ���ϰ�
// ������ ��ȭ�� ���� State�� �����Ѵ�.
bool GPlayer::Interaction()
{
	// ��ó�� ���ڰ� �ִ��� Ȯ���Ѵ�.
	// ���ڰ� �ִٸ� ���� ���·� ��ȯ��.
	if (BoxCheck())
	{
		//m_FSM->ChanageState(L"OpenBox");
		return true;
	}

	// ���� �������� ������ ��Ȳ���� Ȯ���Ѵ�.
	// ItemCheck
	if (ItemCheck())
	{
		m_FSM->ChanageState(L"UseItem");
		return true;
	}
	return false;
}

// ��ó�� ���� �ִ��� Ȯ���Ѵ�.
// ���ڰ� ��ó�� �ִٸ� BoxCheck�� true�� ��ȯ�Ѵ�.
bool GPlayer::BoxCheck()
{
	return false;
}

// �������� ������ ��Ȳ���� Ȯ���Ѵ�.
// ������ ��Ȳ�̶�� m_PlayerUseItem = true�� �Ǹ�
// ItemCheck�� True�� ��ȯ�Ѵ�.
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
	// �ش� �������� ���� ���� �ʴٸ� false�� ��ȯ
	if(!m_PlayerItems[(int)PLAYER_ITEM::HOOK])
		return false;

	// �÷��̾� �ٷ� ���ʿ� ������ �ִٸ� true�� ��ȯ
	
	vector<GGameObject*> vecPlatform = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)LAYER_TYPE::PLATFORM)->GetObjects();

	GPlatform* AbovePF = nullptr;

	for (int i = 0; i < vecPlatform.size(); ++i)
	{
		GPlatform* pPlatform = vecPlatform[i]->GetComponent<GPlatform>();
		assert(pPlatform);
		
		Vector3 PFPos = pPlatform->Transform()->GetWorldPos();
		Vector3 PFScale = pPlatform->Transform()->GetWorldScale();
		
		// �� ������ ������ ȸ������ �����Ƿ� �ܼ��ϰ� ����Ѵ�.

		// �ش� �÷����� �÷��̾� ���� ���ٸ� �Ѿ��.
		if (PFPos.y <= Transform()->GetWorldPos().y)
			continue;
		
		// �÷��̾ �ش� �÷��� �ȿ� ���ٸ� �Ѿ��.
		if (Transform()->GetWorldPos().x < PFPos.x - PFScale.x / 2 || PFPos.x + PFScale.x / 2 <  Transform()->GetWorldPos().x)
			continue;

		if (AbovePF == nullptr || PFPos.y < AbovePF->Transform()->GetWorldPos().y)
		{
			AbovePF = pPlatform;
		}
	}

	if (AbovePF == nullptr)
		return false;

	// ���� �÷����� �ƴ϶�� ����
	if (AbovePF->GetPlatformType() != PLATFORM_TYPE::WOOD)
		return false;

	return true;
}

bool GPlayer::Bomb()
{
	// �ش� �������� ���� ���� �ʴٸ� false�� ��ȯ
	if (!m_PlayerItems[(int)PLAYER_ITEM::BOMB])
		return false;

	// ��ó�� ������ �ݰ�
}

bool GPlayer::LandCheck()
{



	return false;
}

bool GPlayer::FallCheck()
{
	return false;
}