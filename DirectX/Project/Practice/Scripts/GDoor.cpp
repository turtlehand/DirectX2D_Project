#include "pch.h"
#include "GDoor.h"

#include <Engine/GTransform.h>

GDoor::GDoor()
	: GInteractable(DOOR)
{
}

GDoor::~GDoor()
{
}


void GDoor::Init()
{
	ADD_BOOL("StartInteract", &m_StartInteract);
	ADD_VECTOR3("Start Pos", &m_StartPos);
	ADD_VECTOR3("End Pos", &m_EndPos);
}

void GDoor::Begin()
{
}

void GDoor::Update()
{
	if (m_SucessInteraction)
		return;

	if (m_StartInteract)
	{
		float ratio = m_Timer / m_Duration;
		Vector3 vPos;
		if (ratio <= 1)
		{
			vPos = m_StartPos * (1 - ratio) + m_EndPos * ratio;
		}
		else
		{
			vPos = m_EndPos;
			m_SucessInteraction = true;
		}

		Transform()->SetRelativePos(vPos);
	}
}

void GDoor::InteractEnter()
{
	if (m_SucessInteraction || m_StartInteract)
		return;
	m_StartInteract = true;
}

void GDoor::SaveToFile(FILE* _File)
{
}

void GDoor::LoadFromFile(FILE* _File)
{
}

