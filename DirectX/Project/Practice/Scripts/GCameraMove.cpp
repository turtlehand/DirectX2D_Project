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
	, m_CamSpeed(500.f)
	, m_Offset(Vector3(0.f,0.f,-100.f))
{
}

GCameraMove::~GCameraMove()
{
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

	Vector3 vPos = m_Target->Transform()->GetWorldPos() + m_Offset;
	Transform()->SetRelativePos(vPos);

}

void GCameraMove::SaveToFile(FILE* _File)
{
	fwrite(&m_CamSpeed, sizeof(float), 1, _File);
}

void GCameraMove::LoadFromFile(FILE* _File)
{
	fread(&m_CamSpeed, sizeof(float), 1, _File);
}
