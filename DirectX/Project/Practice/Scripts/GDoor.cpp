#include "pch.h"
#include "GDoor.h"

#include <Engine/GTimeManager.h>
#include <Engine/GTransform.h>

GDoor::GDoor()
	: GInteractable(DOOR)
	, m_StartInteract(false)
	, m_StartPos()
	, m_EndPos()
	, m_Duration(5.f)
	, m_Timer(0.f)
{
}

GDoor::GDoor(const GDoor& _Origin)
	: GInteractable(DOOR)
	, m_StartInteract(false)
	, m_StartPos(_Origin.m_StartPos)
	, m_EndPos(_Origin.m_EndPos)
	, m_Duration(_Origin.m_Duration)
	, m_Timer(0.f)
{
}

GDoor::~GDoor()
{
}


void GDoor::Init()
{
	ADD_BOOL("StartInteract", &m_StartInteract);
	ADD_FLOAT("Duration", &m_Duration);
	ADD_VECTOR3("Start Pos", &m_StartPos);
	ADD_VECTOR3("End Pos", &m_EndPos);

	m_StartPos = Transform()->GetRelativePos();
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
		m_Timer += DT;
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
	fwrite(&m_EndPos, sizeof(Vector3), 1, _File);
	fwrite(&m_Duration, sizeof(float), 1, _File);
}

void GDoor::LoadFromFile(FILE* _File)
{
	fread(&m_EndPos, sizeof(Vector3), 1, _File);
	fread(&m_Duration, sizeof(float), 1, _File);
}

