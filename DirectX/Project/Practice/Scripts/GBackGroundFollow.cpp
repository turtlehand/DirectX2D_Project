#include "pch.h"
#include "GBackGroundFollow.h"

#include <Engine/GTransform.h>

#include <Engine/GLevelManager.h>
#include <Engine/GLayer.h>
#include <Engine/GLevel.h>

void GBackGroundFollow::Init()
{
	ADD_FLOAT("Speed", &m_Speed);
}

void GBackGroundFollow::Begin()
{
	m_InitPos = Transform()->GetWorldPos();
	m_Target = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer((int)LAYER_TYPE::DEFAULT)->FindObject(L"MainCamera");
}

void GBackGroundFollow::Update()
{
	if (!m_Target)
		return;


	Vector3 vPos = m_InitPos + m_Target->Transform()->GetWorldPos() * m_Speed;
	vPos.z = Transform()->GetWorldPos().z;
	Transform()->SetRelativePos(vPos);

}

void GBackGroundFollow::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void GBackGroundFollow::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}

GBackGroundFollow::GBackGroundFollow()
	: GScript(BACKGROUNDFOLLOW)
	, m_Target(nullptr)
	, m_Speed(1)
{
}

GBackGroundFollow::~GBackGroundFollow()
{
}
