#include "pch.h"
#include "GObjectBasic.h"

#include <Engine/GTransform.h>
#include <Engine/GRigidBody2D.h>

GObjectBasic::GObjectBasic(SCRIPT_TYPE _Type)
	: GScript(_Type)
	, m_vecPlatform()
	, m_IsGround(false)
	, m_IsCeiling(false)
	, m_GravityScale(200.f)
	, m_FlinchForce(60, 40)
	, m_MaxHP(1)
	, m_HP(1)
{
}

GObjectBasic::GObjectBasic(const GObjectBasic& _Origin)
	: GScript((SCRIPT_TYPE)_Origin.GetScriptType())
	, m_vecPlatform()
	, m_IsGround(false)
	, m_IsCeiling(false)
	, m_GravityScale(_Origin.m_GravityScale)
	, m_FlinchForce(_Origin.m_FlinchForce)
	, m_MaxHP(_Origin.m_MaxHP)
	, m_HP(_Origin.m_HP)
{
}

GObjectBasic::~GObjectBasic()
{
}


void GObjectBasic::SetMoveDirection(int _Direction)
{
	if (_Direction == 0)
		return;
	Vector3 DefaultScale = Transform()->GetRelativeScale();
	DefaultScale.x = _Direction * fabs(DefaultScale.x);
	Transform()->SetRelativeScale(DefaultScale);
}

void GObjectBasic::GroundEnter()
{
	RigidBody2D()->SetGravity(0);
	RigidBody2D()->SetVelocityY(0);
}

void GObjectBasic::GroundExit()
{
	RigidBody2D()->SetGravity(m_GravityScale);
}

void GObjectBasic::CeilingEnter()
{
	// 천장에 닿았을 때 만약 속도가 양수라면 0으로 바꾼다.
	if (RigidBody2D()->GetVelocityY() > 0)
		RigidBody2D()->SetVelocityY(0);
}

void GObjectBasic::CeilingExit()
{

}