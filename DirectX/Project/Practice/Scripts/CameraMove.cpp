#include "pch.h"
#include "CameraMove.h"
#include <Engine/GTimeManager.h>
#include <Engine/GKeyManager.h>
#include <Engine/GTransform.h>

CameraMove::CameraMove()
	: GScript(CAMERAMOVE)
	, m_CamSpeed(500.f)
{
}

CameraMove::~CameraMove()
{
}

void CameraMove::Update()
{
	// 이동
	Vector3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::FRONT) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::S))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::FRONT) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::D))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::A))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::RIGHT) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::SPACE))
		vPos += Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * DT * m_CamSpeed;
	if (KEY_PRESSED(KEY::CTRL))
		vPos -= Transform()->GetRelativeDirection(DIRECTION_TYPE::UP) * DT * m_CamSpeed;

	GameObject()->Transform()->SetRelativePos(vPos);

	// 회전
	if (!KEY_PRESSED(KEY::RBTN))
		return;

	Vector3 vRot = Transform()->GetRelativeRotation();

	vRot += 180.f * DT * Vector3(GKeyManager::GetInst()->GetMouseDir().y, GKeyManager::GetInst()->GetMouseDir().x, 0.f);

	Transform()->SetRelativeRotation(vRot);

}

void CameraMove::SaveToFile(FILE* _File)
{
	fwrite(&m_CamSpeed, sizeof(float), 1, _File);
}

void CameraMove::LoadFromFile(FILE* _File)
{
	fread(&m_CamSpeed, sizeof(float), 1, _File);
}
