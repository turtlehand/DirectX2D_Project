#include "pch.h"
#include "GCameraLimit.h"

#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include <Engine/GCollisionManager.h>

#include <Engine/components.h>

#include "GPlayer.h"
#include "GCameraMove.h"

GCameraLimit::GCameraLimit()
	:GScript(CAMERALIMIT)
{
}

GCameraLimit::~GCameraLimit()
{
}

void GCameraLimit::Init()
{
}

void GCameraLimit::Begin()
{
	m_Target = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)LAYER_TYPE::PLAYER)->FindObject(L"Player");
	assert(m_Target);

	GGameObject* Camera = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)LAYER_TYPE::DEFAULT)->FindObject(L"MainCamera");
	m_MainCamera = Camera->GetComponent<GCameraMove>();
	assert(m_MainCamera);
}

void GCameraLimit::Update()
{

	DrawDebugRect(Vector4(0.f, 0.f, 1.f, 1.f), Transform()->GetRelativePos(), Transform()->GetRelativeScale(), Vector3(0.f, 0.f, 0.f));

	if (m_MainCamera == nullptr || m_Target == nullptr)
		return;


	Vector3 TargetPos = m_Target->Transform()->GetRelativePos();

	Vector3 Pos = Transform()->GetRelativePos();
	Vector3 Scale = Transform()->GetRelativeScale() /2;

	if (Pos.x - Scale.x < TargetPos.x && TargetPos.x < Pos.x + Scale.x && Pos.y - Scale.y < TargetPos.y && TargetPos.y < Pos.y + Scale.y)
	{
		m_MainCamera->SetCenter(Transform()->GetRelativePos());
		m_MainCamera->SetMapSize(Scale* 2);
	}

}

void GCameraLimit::SaveToFile(FILE* _File)
{
}

void GCameraLimit::LoadFromFile(FILE* _File)
{
}


