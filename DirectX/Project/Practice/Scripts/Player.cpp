#include "pch.h"
#include "Player.h"
#include <Engine/GGameObject.h>
#include <Engine/components.h>
#include <Engine/GTimeManager.h>
#include <Engine/GKeyManager.h>
#include <Engine/GRenderManager.h>
#include <Engine/GAssetManager.h>
#include <Engine/GTaskManager.h>



Player::Player()
	: GScript(PLAYER)
	, m_Speed(100.f)
	, m_PaperBurn(-1.f)
	, m_Prefab(nullptr)
{

}

Player::~Player()
{
}

void Player::Init()
{
	ADD_FLOAT("Speed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Missile", &m_Prefab);
}

void Player::Begin()
{
	m_Prefab = GAssetManager::GetInst()->FindAsset<GPrefab>(L"Missile");
}

void Player::Update()
{
	if (RigidBody2D() == nullptr)
		return;

	Vector3 vRotation = Transform()->GetRelativeRotation();
	Vector3 vScale = Transform()->GetWorldScale();

	// Ű�Է¿� ���� �簢���� �����̵��� �Ѵ�.
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

	if (KEY_DOWN(KEY::ENTER))
	{
		if(m_Prefab != nullptr)
			Instantiate(m_Prefab, Transform()->GetWorldPos());
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

void Player::OnTriggerEnter(GCollider2D* _Other)
{
	Renderer()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, 1);
}

void Player::OnTriggerStay(GCollider2D* _Other)
{
}

void Player::OnTriggerExit(GCollider2D* _Other)
{
	Renderer()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, 0);
}

void Player::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
	fwrite(&m_PaperBurn, sizeof(float), 1, _File);
	SaveAssetRef(m_Prefab, _File);
}

void Player::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
	fread(&m_PaperBurn, sizeof(float), 1, _File);
	m_Prefab = LoadAssetRef<GPrefab>(_File);
}
