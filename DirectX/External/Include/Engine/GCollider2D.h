#pragma once
#include "GComponent.h"

enum class COLLIDER_TYPE
{
	STATIC,
	DYNAMIC,
};

class GCollider2D :
	public GComponent
{
private:

	//b2BodyDef m_BodyDef;
	b2BodyId m_BodyId;
	b2ShapeId m_ShapeId;

	b2BodyType m_BodyType;
	bool m_Sensor;

	Vector2 m_Scale;
	bool m_FixedRotation;

	// Dynmaic
	float m_Mass;
	float m_Friction;

	bool m_NotifyParent;

public:
	bool IsSensor() { return m_Sensor; }
	void SetSensor(bool _Sensor);

	void WakeUp() { b2Body_SetAwake(m_BodyId, true); }

	Vector2 GetPos() { b2Vec2 b2V = b2Body_GetPosition(m_BodyId); return Vector2(b2V.x, b2V.y); }

	Vector2 GetScale() { return m_Scale; }
	void SetScale(Vector2 _Scale);
	void SetScale(float _x, float _y);

	b2BodyType GetBodyType() { return m_BodyType; }
	void SetBodyType(b2BodyType _BodyType);

	Vector2 GetVelocity();
	void SetVelocity(Vector2 _Velocity);
	
	float GetMass() { return m_Mass; }
	void SetMass(float _Mass);

	float GetFriction() { return m_Friction; }
	void SetFriction(float _Friction);
	
	bool GetNotifyParent() { return m_NotifyParent; }
	void SetNotifyParent(bool _NotrifyParent) { m_NotifyParent = _NotrifyParent; } // 이 오브젝트가 충돌 시 부모 오브젝트들에게 영향을 줄지 여부를 설정합니다.

	bool GetFixedRotation() { return m_FixedRotation; }
	void SetFixedRotation(bool _Fixed) { b2Body_SetFixedRotation(m_BodyId, _Fixed); m_FixedRotation = _Fixed; }

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void FinalUpdate() override;

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	virtual void OnCollisionEnter(GCollider2D* _Other);
	virtual void OnCollisionStay(GCollider2D* _Other);
	virtual void OnCollisionExit(GCollider2D* _Other);

	virtual void OnTriggerEnter(GCollider2D* _Other);
	virtual void OnTriggerStay(GCollider2D* _Other);
	virtual void OnTriggerExit(GCollider2D* _Other);

private:
	void NotifyEnter(GCollider2D* _Other);
	void NotifyStay(GCollider2D* _Other);
	void NotifyExit(GCollider2D* _Other);

public:
	CLONE(GCollider2D);

public:
	GCollider2D();
	GCollider2D(const GCollider2D& _Origin);
	~GCollider2D();

	friend class GTransform;
};

