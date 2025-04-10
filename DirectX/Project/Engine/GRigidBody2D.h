#pragma once
#include "GComponent.h"
class GRigidBody2D :
	public GComponent
{
private:
	Vector2 m_Velocity;
	float m_Mass;
	float m_Gravity;
	Vector2 m_Force;
	float m_Friction;

public:
	void SetVelocity(Vector2 _Velocity) { m_Velocity = _Velocity; };
	Vector2 GetVelocity() { return m_Velocity; }
	void AddVelocity(Vector2 _Velocity) { m_Velocity += _Velocity; };

	void SetMass(float _Mass) { m_Mass = _Mass; }
	float GetMass() { return m_Mass; }

	void SetGravity(float _Gravity) { m_Gravity = _Gravity; }
	float GetGravity() { return m_Gravity; }

	void AddForce(Vector2 _Force) {	m_Force += _Force; }

	float GetFriction() { return m_Friction; }
	void SetFriction(float _Friction) { m_Friction = _Friction; }

public:
	virtual void FinalUpdate() override;

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

private:
	void CalFriction();

public:
	CLONE(GRigidBody2D)

public:
	GRigidBody2D();
	~GRigidBody2D();
};

