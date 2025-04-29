#include "pch.h"
#include "GLord.h"

#include <Engine/GScript.h>

#include <Engine/components.h>
#include <Engine/GAssetManager.h>
#include <Engine/GCollisionManager.h>
#include <Engine/GTimeManager.h>
#include <Engine/GSound.h>

#include "GGameManager.h"

GLord::GLord()
	: GObjectBasic(LORD)
	, m_IsDead(false)
	, m_FindPlayer(false)
	, m_Timer(0.f)
	, m_DetectPos(Vector3(10, 0.f, 0.f))
	, m_DetectScale(Vector2(35.f, 20.f))

{
}

GLord::GLord(const GLord& _Origin)
	: GObjectBasic(LORD)
	, m_IsDead(false)
	, m_FindPlayer(false)
	, m_Timer(0.f)
	, m_DetectPos(Vector3(10, 0.f, 0.f))
	, m_DetectScale(Vector2(35.f, 20.f))
{

}

GLord::~GLord()
{
}


void GLord::Init()
{
}

void GLord::Begin()
{
}

void GLord::Update()
{
	GObjectBasic::Update();
	FindPlayer();
	Dead();
}

void GLord::OnOverlapEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetLayer() == (int)LAYER_TYPE::PLAYER_ATTACK)
	{
		// 피 파티클 생성
		GGameObject* pBlood = GAssetManager::GetInst()->Load<GPrefab>(L"Prefab\\Blood.prefab", L"Prefab\\Blood.prefab")->Instantiate();
		pBlood->Transform()->SetRelativePos(Vector3(-0.3f, 0.f, 0.f));
		SpawnGameObject(pBlood);
		GameObject()->SetChild(pBlood);

		// 죽음
		if (!m_IsDead)
		{
			m_IsDead = true;
			m_Timer = 0.f;

			Ptr<GSound> Stab = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\StabSomeone.wav", L"Sound\\AudioClip\\StabSomeone.wav");
			Ptr<GSound> KillDarkLord = GAssetManager::GetInst()->Load<GSound>(L"Sound\\AudioClip\\KillDarkLord.wav", L"Sound\\AudioClip\\KillDarkLord.wav");

			Stab->Play(1, GGameManager::GetInst()->GetEffect_Volume(), false);
			KillDarkLord->Play(1, GGameManager::GetInst()->GetEffect_Volume(), false);

			// 엔딩
			GGameManager::GetInst()->GameEnding(ENDING_TYPE::Cowardly_Hero);
		}
	}
}

void GLord::SaveToFile(FILE* _File)
{
}

void GLord::LoadFromFile(FILE* _File)
{
}

void GLord::FindPlayer()
{
	if (m_FindPlayer)
	{
		RigidBody2D()->SetVelocityX(GetDirection() * 30);
		return;
	}
		

	Vector3 Pos = Transform()->GetWorldPos();
	Vector3 Scale = Transform()->GetWorldScale();

	// 플레이어 탐지
	DrawDebugRect(Vector4(1.f, 0.f, 0.f, 1.f), Pos + GetDirection() * m_DetectPos, Vector3(m_DetectScale.x, m_DetectScale.y,0), Vector3(0.f, 0.f, 0.f));

	m_FindPlayer = GCollisionManager::GetInst()->Box_Casting(Pos + GetDirection() * m_DetectPos
		, m_DetectScale
		, (1 << (int)LAYER_TYPE::PLAYER));

	if (m_FindPlayer)
	{
		// 엔딩
		RigidBody2D()->SetVelocityX(GetDirection() * 30);
		FlipbookRender()->Play(1);
		GGameManager::GetInst()->GameEnding(ENDING_TYPE::Execution);
		return;
	}
}

void GLord::Dead()
{
	if (m_IsDead)
	{
		if (0 < m_Timer && m_Timer < 0.1f)
		{
			float ratio = m_Timer / 0.1;
			Transform()->SetRelativeRotation(0.f, 0.f, -90 * ratio);
		}
		else if (0.1f < m_Timer && m_Timer < 0.2f)
		{
			float ratio = (m_Timer - 0.1f) / 0.1;
			Transform()->SetRelativeRotation(0.f, 0.f, -90.f * (1 - ratio) + -75.f * ratio);
		}
		else if (0.2f < m_Timer && m_Timer < 0.375f)
		{
			float ratio = (m_Timer - 0.2f) / 0.175f;
			Transform()->SetRelativeRotation(0.f, 0.f, -90.f * ratio + -75.f * (1 - ratio));
		}
		else if (0.375 < m_Timer)
		{
			Transform()->SetRelativeRotation(0.f, 0.f, -90);
		}
		m_Timer += DT;
	}
}

