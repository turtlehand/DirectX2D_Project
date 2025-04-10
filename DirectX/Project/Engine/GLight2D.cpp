#include "pch.h"
#include "GLight2D.h"

#include "GRenderManager.h"
#include "GTransform.h"
#include "GTimeManager.h"

GLight2D::GLight2D()
	: GComponent(COMPONENT_TYPE::LIGHT2D)
	, m_Info{}
{
	m_Info.Type = LIGHT_TYPE::DIRECTIONAL;
}

GLight2D::~GLight2D()
{
}

void GLight2D::FinalUpdate()
{
	GRenderManager::GetInst()->RegisterLight2D(this);

	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.Dir = Transform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);

	//Vector3 vWorldPos = Transform()->GetWorldPos();
	//DrawDebugRect(Vector4(0.8, 0.4f, 0.4f, 1.f), vWorldPos, Vector3(20.f, 20.f, 1.f), Vector3(0.f, 0.f, 0.f), true);
}

void GLight2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLight2DInfo), 1, _File);
}

void GLight2D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLight2DInfo), 1, _File);
}


