#pragma once
#include "GComponent.h"
class GRigidBody2D :
	public GComponent
{
private:
	Vector2 m_Velocity;
	float m_Mass;
	float m_Gravity;
	float m_TerminalSpeed;		// 종단 속도
	Vector2 m_Force;
	float m_Friction;

public:
	void SetVelocity(Vector2 _Velocity) { m_Velocity = _Velocity; };
	void SetVelocity(float _x, float _y) { m_Velocity = Vector2(_x, _y); }
	void SetVelocityX(float _x) { m_Velocity.x = _x; }
	void SetVelocityY(float _y) { m_Velocity.y = _y; }

	Vector2 GetVelocity() { return m_Velocity; }
	float GetVelocityX() { return m_Velocity.x; }
	float GetVelocityY() { return m_Velocity.y; }

	void AddVelocity(Vector2 _Velocity) { m_Velocity += _Velocity; };
	void AddVelocity(float _x, float _y) { m_Velocity += Vector2(_x, _y); }
	void AddVelocityX(float _x) { m_Velocity.x += _x; }
	void AddVelocityY(float _y) { m_Velocity.y += _y; }

	void SetMass(float _Mass) { m_Mass = _Mass; }
	float GetMass() { return m_Mass; }

	void SetGravity(float _Gravity) { m_Gravity = _Gravity; }
	float GetGravity() { return m_Gravity; }

	void SetTerminalSpeed(float _TerminalSpeed) { m_TerminalSpeed = _TerminalSpeed; }
	float GetTerminalSpeed() { return m_TerminalSpeed; }

	void AddForce(Vector2 _Force) {	m_Force += _Force; }

	float GetFriction() { return m_Friction; }
	void SetFriction(float _Friction) { m_Friction = _Friction; }

public:
	virtual void FinalUpdate() override;

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
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

