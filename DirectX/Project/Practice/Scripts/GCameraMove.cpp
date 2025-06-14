#include "pch.h"
#include "GCameraMove.h"
#include <Engine/GTimeManager.h>
#include <Engine/GKeyManager.h>
#include <Engine/GTransform.h>
#include <Engine/GCamera.h>

#include <Engine/GLevelManager.h>
#include <Engine/GLayer.h>
#include <Engine/GLevel.h>

GCameraMove::GCameraMove()
	: GScript(CAMERAMOVE)
	, m_CamSpeed(0.7f)
	, m_Offset(Vector3(0.f,0.f,-100.f))
	, m_PostCenter()
	, m_Center()
	, m_PostMapSize(256.f, 256.f, 0.f)
	, m_MapSize(256.f, 256.f,0.f)
{
}

GCameraMove::~GCameraMove()
{
}

void GCameraMove::Init()
{
	ADD_FLOAT("Speed", &m_CamSpeed);
	ADD_VECTOR3("Center", &m_Center);
	ADD_VECTOR3("Size", &m_MapSize)
}

void GCameraMove::Begin()
{
	m_Target = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)LAYER_TYPE::PLAYER)->FindObject(L"Player");
}

void GCameraMove::Update()
{

	if (!m_Target)
		return;

	// 이동
	if (KEY_PRESSED(KEY::D))
		m_Offset += Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::A))
		m_Offset -= Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::W))
		m_Offset += Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::S))
		m_Offset -= Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * DT * m_CamSpeed;

	float ScaleX = Camera()->GetOrthoScaleX();

	if (KEY_PRESSED(KEY::PLUS))
		Camera()->SetOrthoScaleX(ScaleX - 1);
	if (KEY_PRESSED(KEY::MINUS))
		Camera()->SetOrthoScaleX(ScaleX + 1);



	// 초기화
	if (KEY_DOWN(KEY::R))
		m_Offset = Vector3(0,0,-100.f);

	LimitCameraArea();

}

void GCameraMove::SaveToFile(FILE* _File)
{
	fwrite(&m_CamSpeed, sizeof(float), 1, _File);
}

void GCameraMove::LoadFromFile(FILE* _File)
{
	fread(&m_CamSpeed, sizeof(float), 1, _File);
}

void GCameraMove::LimitCameraArea()
{
	Vector3 TargetPos = m_Target->Transform()->GetWorldPos();

	DrawDebugRect(Vector4(0.f, 0.f, 1.f, 1.f), m_Center, m_MapSize, Vector3(0.f, 0.f, 0.f));

	m_Center = m_Center + (m_PostCenter - m_Center) * DT * m_CamSpeed;
	m_MapSize = m_MapSize + (m_PostMapSize - m_MapSize) * DT * m_CamSpeed;

	float CameraSize = Camera()->GetOrthoScaleX();
	Camera()->SetOrthoScaleX(CameraSize + (m_PostCameraSize - CameraSize) * DT * m_CamSpeed);

	float lx = m_MapSize.x / 2 - Camera()->GetOrthoScaleX() / 2;
	float ly = m_MapSize.y / 2 - (Camera()->GetOrthoScaleX() / Camera()->GetAspectRatio()) / 2;
	if (lx < 0 || ly < 0)
	{
		lx = 0.f;
		ly = 0.f;
	}

	float clampX = std::clamp(TargetPos.x, -lx + m_Center.x, lx + m_Center.x);
	float clampY = std::clamp(TargetPos.y, -ly + m_Center.y, ly + m_Center.y);

	TargetPos = Vector3(clampX, clampY, -100.f);

	Transform()->SetRelativePos(TargetPos);

	
}
