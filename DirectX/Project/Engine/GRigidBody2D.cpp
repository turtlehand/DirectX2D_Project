#include "pch.h"
#include "GRigidBody2D.h"

#include "GGameObject.h"
#include "GTransform.h"
#include "GCollider2D.h"

#include "GTimeManager.h"

GRigidBody2D::GRigidBody2D()
	:GComponent(COMPONENT_TYPE::RIGIDBODY2D),
	m_Velocity(),
	m_Mass(1),
	m_Gravity(0),
	m_Force()
{

}

GRigidBody2D::~GRigidBody2D()
{

}

void GRigidBody2D::FinalUpdate()
{

	Vector3 vPos = Transform()->GetWorldPos();
	Vector2 vAccel = m_Force / m_Mass;

	CalFriction();
	
	vAccel.y -= m_Gravity * DT;

	m_Velocity += vAccel;
	
	vPos += Vector3(m_Velocity.x, m_Velocity.y, 0) * DT;

	Transform()->SetRelativePos(vPos);

	m_Force = Vector2(0.f, 0.f);
}

void GRigidBody2D::CalFriction()
{
	if (m_Gravity == 0)
	{
		float Speed = m_Velocity.Length();

		if (Speed <= 0)
			return;

		Speed = max(0, Speed - m_Friction * DT);

		m_Velocity.Normalize();
		m_Velocity = m_Velocity * Speed;
	}
	else
	{
		float Speed = fabs(m_Velocity.x);
		

		if (Speed <= 0)
			return;

		Speed = max(0, Speed - m_Friction * DT);
		m_Velocity.x = Speed * (m_Velocity.x < 0 ? -1 : 1);
	}

}

void GRigidBody2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Velocity, sizeof(Vector2), 1, _File);
	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_Gravity, sizeof(float), 1, _File);
	fwrite(&m_Force, sizeof(Vector2), 1, _File);
	fwrite(&m_Friction, sizeof(float), 1, _File);
}

void GRigidBody2D::LoadFromFile(FILE* _File)
{
	fread(&m_Velocity, sizeof(Vector2), 1, _File);
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_Gravity, sizeof(float), 1, _File);
	fread(&m_Force, sizeof(Vector2), 1, _File);
	fread(&m_Friction, sizeof(float), 1, _File);
}



