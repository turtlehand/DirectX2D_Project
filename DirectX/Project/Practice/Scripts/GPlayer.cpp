#include "pch.h"
#include "GPlayer.h"
#include <Engine/GGameObject.h>
#include <Engine/components.h>
#include <Engine/GTimeManager.h>
#include <Engine/GKeyManager.h>
#include <Engine/GRenderManager.h>
#include <Engine/GAssetManager.h>
#include <Engine/GTaskManager.h>

#include "GFSM.h"

#include "GPlayerDefaultState.h"
#include "GPlayerWalkState.h"

GPlayer::GPlayer()
	: GScript(PLAYER)
	, m_FSM(nullptr)
	, m_Speed(100.f)
{

}

GPlayer::~GPlayer()
{
}

void GPlayer::Init()
{
	ADD_FLOAT("Speed", &m_Speed);

	m_FSM->AddState(L"Default", new GPlayerDefaultState);
	m_FSM->AddState(L"Walk", new GPlayerWalkState);
}

void GPlayer::Begin()
{

}

void GPlayer::Update()
{
	if (RigidBody2D() == nullptr)
		return;

	Vector3 vRotation = Transform()->GetRelativeRotation();
	Vector3 vScale = Transform()->GetWorldScale();

	// 키입력에 따라서 사각형이 움직이도록 한다.
	if (KEY_PRESSED(KEY::LEFT))
	{
		RigidBody2D()->AddForce(Vector2(-1, 0) * m_Speed * DT);
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		RigidBody2D()->AddForce(Vector2(1, 0) * m_Speed * DT);
	}

	if (KEY_PRESSED(KEY::UP))
	{
		RigidBody2D()->AddForce(Vector2(0, 1) * m_Speed * DT);
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		RigidBody2D()->AddForce(Vector2(0, -1) * m_Speed * DT);
	}

	if (KEY_PRESSED(KEY::Q))
	{
		vRotation.z -= 90.f * DT;
	}

	if (KEY_PRESSED(KEY::E))
	{
		vRotation.z += 90.f * DT;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		vScale *= 1 + (0.1f * DT);
		//GameObject()->Renderer()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAM);
	}
	else if (KEY_UP(KEY::Z))
	{
		vScale = Vector3(50.f, 50.f, 1.f);
		//GameObject()->Renderer()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
	}



	//DrawDebugCircle(Vector4(0.f, 1.f, 0.f, 1.f), Transform()->GetWorldPos(), 25.f, Transform()->GetWorldRotation());
	//DrawDebugRect(Vector4(0.f, 1.f, 0.f, 1.f), Transform()->GetWorldPos() , Transform()->GetWorldScale() + Vector3(10.f, 10.f, 0.f), Transform()->GetWorldRotation());

	/*
	if (KEY_PRESSED(KEY::W))
	{
		vPos.z += 10.f * DT;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos.z -= 10.f * DT;
	}
	*/
}

void GPlayer::OnTriggerEnter(GCollider2D* _Other)
{
	Renderer()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, 1);
}

void GPlayer::OnTriggerStay(GCollider2D* _Other)
{
}

void GPlayer::OnTriggerExit(GCollider2D* _Other)
{
	Renderer()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, 0);
}

void GPlayer::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void GPlayer::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
