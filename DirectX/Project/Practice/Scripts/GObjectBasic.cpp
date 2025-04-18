#include "pch.h"
#include "GObjectBasic.h"

#include <Engine/GRigidBody2D.h>

GObjectBasic::GObjectBasic(SCRIPT_TYPE _Type)
	:GScript(_Type)
	, m_GravityScale(200.f)
	, m_FlinchForce(60, 40)
{
}

GObjectBasic::~GObjectBasic()
{
}


void GObjectBasic::GroundEnter()
{
	RigidBody2D()->SetGravity(0);
}

void GObjectBasic::GroundExit()
{
	RigidBody2D()->SetGravity(m_GravityScale);
}

void GObjectBasic::CeilingEnter()
{
	// õ�忡 ����� �� ���� �ӵ��� ������ 0���� �ٲ۴�.
	if (RigidBody2D()->GetVelocityY() > 0)
		RigidBody2D()->SetVelocityY(0);
}

void GObjectBasic::CeilingExit()
{

}