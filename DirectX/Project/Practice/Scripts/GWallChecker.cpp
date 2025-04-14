#include "pch.h"
#include "GWallChecker.h"

#include <Engine/components.h>

#include "GPlayer.h"

GWallChecker::GWallChecker()
	:GScript(WALLCHECKER)
{
}

GWallChecker::~GWallChecker()
{
}


void GWallChecker::Begin()
{
	GGameObject* ParentObject = GameObject();
	while (true)
	{
		ParentObject = ParentObject->GetParent();
		assert(ParentObject != nullptr);

		m_Owner = ParentObject->GetComponent<GObjectBasic>();
		if (m_Owner != nullptr)
			return;
	}


}

void GWallChecker::Update()
{
}

void GWallChecker::OnTriggerEnter(GCollider2D* _Other)
{
	if (m_Owner == nullptr)
		return;

	if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
		return;

	Vector3 OtherPos = _Other->Transform()->GetWorldPos();
	Vector3 OtherScale = _Other->Transform()->GetWorldScale();
	Vector3 ThisPos = GameObject()->Transform()->GetWorldPos();
	Vector3 ThisScale = GameObject()->Transform()->GetWorldScale();

	if (ThisPos.x < OtherPos.x)
	{
		++m_Owner->m_IsRightWall;
	}
	else
	{
		++m_Owner->m_IsLeftWall;
	}
}

void GWallChecker::OnTriggerStay(GCollider2D* _Other)
{
}

void GWallChecker::OnTriggerExit(GCollider2D* _Other)
{
	if (m_Owner == nullptr)
		return;

	if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
		return;

	Vector3 OtherPos = _Other->Transform()->GetWorldPos();
	Vector3 OtherScale = _Other->Transform()->GetWorldScale();
	Vector3 ThisPos = GameObject()->Transform()->GetWorldPos();
	Vector3 ThisScale = GameObject()->Transform()->GetWorldScale();

	if (ThisPos.x < OtherPos.x)
	{
		--m_Owner->m_IsRightWall;
	}
	else
	{
		--m_Owner->m_IsLeftWall;
	}
}

void GWallChecker::SaveToFile(FILE* _File)
{
}

void GWallChecker::LoadFromFile(FILE* _File)
{
}
