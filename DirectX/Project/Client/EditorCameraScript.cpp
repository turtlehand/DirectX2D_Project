#include "pch.h"
#include "EditorCameraScript.h"

#include <Engine/GKeyManager.h>
#include <Engine/GTimeManager.h>

#include <Engine/GTransform.h>
#include <Engine/GCamera.h>

EditorCameraScript::EditorCameraScript()
	: GScript(-1)
	, m_CamSpeed(500.f)
	, m_Cam(nullptr)
{
}

EditorCameraScript::~EditorCameraScript()
{
}

void EditorCameraScript::Update()
{
	
	// 카메라 컴포넌트가 없으면 종료
	if (m_Cam == nullptr)
	{
		m_Cam = GameObject()->GetComponent<GCamera>();
	}

	if (PROJECTION_TYPE::PERSPECTIVE == m_Cam->GetProjType())
		Move_Perspective();
	else
		Move_OrthoGraphic();



}

void EditorCameraScript::Move_Perspective()
{
	// 이동
	Vector3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::FRONT) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::S))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::FRONT) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::D))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::A))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::SPACE))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::CTRL))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * ENGINEDT * m_CamSpeed;

	GameObject()->Transform()->SetRelativePos(vPos);

	// 회전
	if (!KEY_PRESSED(KEY::RBTN))
		return;

	Vector3 vRot = Transform()->GetRelativeRotation();

	vRot += 180.f * ENGINEDT * Vector3(GKeyManager::GetInst()->GetMouseDir().y, GKeyManager::GetInst()->GetMouseDir().x, 0.f);

	Transform()->SetRelativeRotation(vRot);
}

void EditorCameraScript::Move_OrthoGraphic()
{
	// 이동
	Vector3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::S))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::D))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * ENGINEDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::A))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * ENGINEDT * m_CamSpeed;

	if (KEY_PRESSED(KEY::PLUS))
	{
		m_Cam->SetOrthoScaleX(m_Cam->GetOrthoScaleX() - 1);
	}
	else if (KEY_PRESSED(KEY::MINUS))
	{
		m_Cam->SetOrthoScaleX(m_Cam->GetOrthoScaleX() + 1);
	}

	GameObject()->Transform()->SetRelativePos(vPos);
}
