#include "pch.h"
#include "GElevator.h"

#include <Engine/GTimeManager.h>
#include <Engine/components.h>

GElevator::GElevator()
	: GInteractable(ELEVATOR)
	, m_StartMove(false)
	, m_BottomPos()
	, m_TopPos()
	, m_Duration(5.f)
	, m_Timer(0.f)
{
}

GElevator::GElevator(const GElevator& _Origin)
	: GInteractable(ELEVATOR)
	, m_StartMove(false)
	, m_BottomPos(_Origin.m_BottomPos)
	, m_TopPos(_Origin.m_TopPos)
	, m_Duration(_Origin.m_Duration)
	, m_Timer(0.f)
{
}

GElevator::~GElevator()
{
}

void GElevator::Init()
{
	ADD_BOOL("Start Move", &m_StartMove);
	ADD_FLOAT("Duration", &m_Duration);
	ADD_VECTOR3("Start Pos", &m_BottomPos);
	ADD_VECTOR3("End Pos", &m_TopPos);

	m_BottomPos = Transform()->GetRelativePos();
}

void GElevator::Begin()
{
}

void GElevator::Update()
{
	//if (m_SucessInteraction)
		//return;

	if (m_StartMove)
	{
		Vector3 vPos;
		if (0 < m_Direction)
		{
			float ratio = m_Timer / m_Duration;
			if (ratio <= 1)
			{
				vPos = m_BottomPos * (1 - ratio) + m_TopPos * ratio;
			}
			else
			{
				vPos = m_TopPos;
				//m_SucessInteraction = true;
				m_StartMove = false;
			}
		}
		else if (m_Direction < 0)
		{
			float ratio = m_Timer / m_Duration;

			if (ratio <= 1)
			{
				vPos = m_TopPos * (1 - ratio) + m_BottomPos * ratio;
			}
			else
			{
				vPos = m_BottomPos;
				//m_SucessInteraction = true;
				m_StartMove = false;
			}
		}

		m_Timer += DT;
		Transform()->SetRelativePos(vPos);
	}
}

void GElevator::InteractEnter()
{
	if (m_StartMove)
		return;
	m_Timer = 0.f;
	m_StartMove = true;
	if (m_BottomPos == Transform()->GetRelativePos())
	{
		m_Direction = 1;
	}
	else if (m_TopPos == Transform()->GetRelativePos())
	{
		m_Direction = -1;
	}
}

void GElevator::SaveToFile(FILE* _File)
{
	fwrite(&m_TopPos, sizeof(Vector3), 1, _File);
	fwrite(&m_BottomPos, sizeof(Vector3), 1, _File);

	fwrite(&m_Duration, sizeof(float), 1, _File);
}

void GElevator::LoadFromFile(FILE* _File)
{
	fread(&m_TopPos, sizeof(Vector3), 1, _File);
	fread(&m_BottomPos, sizeof(Vector3), 1, _File);

	fread(&m_Duration, sizeof(float), 1, _File);
}
