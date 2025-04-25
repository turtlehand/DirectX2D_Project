#include "pch.h"
#include "GPlatform.h"

#include <Engine/GTileRender.h>

GPlatform::GPlatform()
	:GScript(PLATFORM)
	,m_PlatformType(PLATFORM_TYPE::DEFAULT)
{
}

GPlatform::~GPlatform()
{
}

void GPlatform::Init()
{
	ADD_INT("Platform Type", &m_PlatformType)
}

void GPlatform::Begin()
{

	if (TileRender())
	{
		// 자식 오브젝트를 콜라이더로 만들지 않는다면 넘어간다.
		if (!TileRender()->IsCollider())
			return;

		const vector<GGameObject*> Colliders = TileRender()->GetColliders();

		for (int i = 0; i < Colliders.size(); ++i)
		{
			GPlatform* ColPlat = Colliders[i]->AddComponent<GPlatform>();
			ColPlat->m_PlatformType = m_PlatformType;
		}

		ChangeGameObjectLayer(GameObject(), (int)LAYER_TYPE::DEFAULT);
	}
}

void GPlatform::Update()
{
}

void GPlatform::SaveToFile(FILE* _File)
{
	fwrite(&m_PlatformType, sizeof(PLATFORM_TYPE), 1, _File);
}

void GPlatform::LoadFromFile(FILE* _File)
{
	fread(&m_PlatformType, sizeof(PLATFORM_TYPE), 1, _File);
}


