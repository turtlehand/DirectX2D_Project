#include "pch.h"
#include "GCollider2D.h"
#include "GScript.h"

#include "GKeyManager.h"
#include "GCollisionManager.h"

#include "GTransform.h"

#include "GLevelManager.h"
#include "GLevel.h"
#include "GLayer.h"

GCollider2D::GCollider2D()
	: GComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_BodyId()
	, m_ShapeId()
	, m_BodyType(b2_staticBody)
	, m_Sensor(false)
	, m_Scale(Vector2(1.f,1.f))
	, m_FixedRotation(true)
	, m_Mass(1.f)
	, m_Friction(0.f)
	, m_NotifyParent(true)
{
	
}

GCollider2D::GCollider2D(const GCollider2D& _Origin)
	: GComponent(_Origin)
	, m_BodyId()
	, m_ShapeId()
	, m_BodyType(_Origin.m_BodyType)
	, m_Sensor(_Origin.m_Sensor)
	, m_Scale(_Origin.m_Scale)
	, m_FixedRotation(_Origin.m_FixedRotation)
	, m_Mass(_Origin.m_Mass)
	, m_Friction(_Origin.m_Friction)
	, m_NotifyParent(_Origin.m_NotifyParent)
{

}

GCollider2D::~GCollider2D()
{
	//b2DestroyShape(m_ShapeId, true);
	if(b2Body_IsValid(m_BodyId))
		b2DestroyBody(m_BodyId);

}

void GCollider2D::Init()
{
	Vector3 vPos = GameObject()->Transform()->GetWorldPos();

	// body 정의 하기
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = m_BodyType;
	bodyDef.fixedRotation = m_FixedRotation;
	bodyDef.position = { vPos.x, vPos.y };

	m_BodyId = b2CreateBody(GCollisionManager::GetInst()->GetWorldId(), &bodyDef);
}

void GCollider2D::Begin()
{
	b2Body_SetAwake(m_BodyId, true);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.enableContactEvents = true;
	shapeDef.enableSensorEvents = true;
	shapeDef.isSensor = m_Sensor;
	

	b2Polygon polygon = b2MakeBox(m_Scale.x / 2, m_Scale.y / 2);

	m_ShapeId = b2CreatePolygonShape(m_BodyId, &shapeDef, &polygon);

	SetMass(m_Mass);
	SetFriction(m_Friction);

	b2Shape_SetUserData(m_ShapeId, this);
}

void GCollider2D::FinalUpdate()
{
	bool Sensor = false;
	if (b2Shape_IsValid(m_ShapeId))
		Sensor = b2Shape_IsSensor(m_ShapeId);


	Vector3 vObjectPos = Transform()->GetWorldPos();
	//Vector3 vObjectRot = Transform()->GetRelativeRotation();
	b2Transform vColliderPos = b2Body_GetTransform(m_BodyId);

	vObjectPos = Vector3(vColliderPos.p.x, vColliderPos.p.y, vObjectPos.z);

	Transform()->m_RelativePosition = vObjectPos;

	DrawDebugRect(Vector4(1.f, 0.f, 0.f, 1.f), vObjectPos, Vector3(m_Scale.x, m_Scale.y,0), Transform()->GetWorldRotation());


}

void GCollider2D::SaveToFile(FILE* _File)
{
	//fwrite(&m_Scale, sizeof(Vector3), 1, _File);
	//fwrite(&m_Offset, sizeof(Vector3), 1, _File);
	//fwrite(&m_NotifyParent, sizeof(bool), 1, _File);
}

void GCollider2D::LoadFromFile(FILE* _File)
{
	//fread(&m_Scale, sizeof(Vector3), 1, _File);
	//fread(&m_Offset, sizeof(Vector3), 1, _File);
	//fread(&m_NotifyParent, sizeof(bool), 1, _File);
}

void GCollider2D::OnCollisionEnter(GCollider2D* _Other)
{
}

void GCollider2D::OnCollisionStay(GCollider2D* _Other)
{
}

void GCollider2D::OnCollisionExit(GCollider2D* _Other)
{
}


void GCollider2D::OnTriggerEnter(GCollider2D* _Other)
{
	//++m_OverlapCount;
	NotifyEnter(_Other);
}

void GCollider2D::OnTriggerStay(GCollider2D* _Other)
{
	NotifyStay(_Other);
}

void GCollider2D::OnTriggerExit(GCollider2D* _Other)
{
	//--m_OverlapCount;
	NotifyExit(_Other);
}

void GCollider2D::NotifyEnter(GCollider2D* _Other)
{
	// 이 컴포넌트의 주인이 소유하고 있는 스크립트에게 충돌을 호출해준다.
	const vector<GScript*>& vecScripts = GameObject()->GetScripts();
	for (size_t i = 0;i < vecScripts.size();++i)
	{
		vecScripts[i]->OnTriggerEnter(_Other);
	}

	// true라면 부모 오브젝트의 스크립트에게도 충돌을 호출해준다.
	if (m_NotifyParent)
	{
		GGameObject* pParent = GameObject()->GetParent();

		while (pParent)
		{
			const vector<GScript*>& vecScripts = pParent->GetScripts();
			for (size_t i = 0;i < vecScripts.size();++i)
			{
				vecScripts[i]->OnTriggerEnter(_Other);
			}
			pParent = pParent->GetParent();
		}
	}
}

void GCollider2D::NotifyStay(GCollider2D* _Other)
{
	// 이 컴포넌트의 주인이 소유하고 있는 스크립트에게 충돌을 호출해준다.
	const vector<GScript*>& vecScripts = GameObject()->GetScripts();
	for (size_t i = 0;i < vecScripts.size();++i)
	{
		vecScripts[i]->OnTriggerStay(_Other);
	}

	// true라면 부모 오브젝트의 스크립트에게도 충돌을 호출해준다.
	if (m_NotifyParent)
	{
		GGameObject* pParent = GameObject()->GetParent();

		while (pParent)
		{
			const vector<GScript*>& vecScripts = pParent->GetScripts();
			for (size_t i = 0;i < vecScripts.size();++i)
			{
				vecScripts[i]->OnTriggerStay(_Other);
			}
			pParent = pParent->GetParent();
		}
	}
}

void GCollider2D::NotifyExit(GCollider2D* _Other)
{
	// 이 컴포넌트의 주인이 소유하고 있는 스크립트에게 충돌을 호출해준다.
	const vector<GScript*>& vecScripts = GameObject()->GetScripts();
	for (size_t i = 0;i < vecScripts.size();++i)
	{
		vecScripts[i]->OnTriggerExit(_Other);
	}

	// true라면 부모 오브젝트의 스크립트에게도 충돌을 호출해준다.
	if (m_NotifyParent)
	{
		GGameObject* pParent = GameObject()->GetParent();

		while (pParent)
		{
			const vector<GScript*>& vecScripts = pParent->GetScripts();
			for (size_t i = 0;i < vecScripts.size();++i)
			{
				vecScripts[i]->OnTriggerExit(_Other);
			}
			pParent = pParent->GetParent();
		}
	}
}

void GCollider2D::SetSensor(bool _Sensor)
{
	if (b2Shape_IsValid(m_ShapeId))
		return;

	m_Sensor = _Sensor;
}

void GCollider2D::SetScale(Vector2 _Scale)
{
	if (b2Shape_IsValid(m_ShapeId))
		return;

	m_Scale = _Scale;
}

void GCollider2D::SetScale(float _x, float _y)
{
	SetScale(Vector2(_x, _y));
}

void GCollider2D::SetBodyType(b2BodyType _BodyType)
{
	b2Body_SetType(m_BodyId, _BodyType);

	m_BodyType = _BodyType;
}

void GCollider2D::SetVelocity(Vector2 _Velocity)
{
	if (m_BodyType == b2_staticBody)
		return;

	b2Body_SetLinearVelocity(m_BodyId, { _Velocity.x, _Velocity.y });
}

Vector2 GCollider2D::GetVelocity()
{
	b2Vec2 Velocity = b2Body_GetLinearVelocity(m_BodyId);
	return Vector2(Velocity.x, Velocity.y);
}

void GCollider2D::SetMass(float _Mass)
{
	if (m_BodyType != b2_dynamicBody)
		return;

	b2MassData massData = b2Body_GetMassData(m_BodyId);
	massData.mass = _Mass;
	b2Body_SetMassData(m_BodyId, massData);

	m_Mass = massData.mass;
}

void GCollider2D::SetFriction(float _Friction)
{
	if (m_BodyType != b2_dynamicBody)
		return;

	m_Friction = _Friction;

	if (!b2Shape_IsValid(m_ShapeId))
		return;

	b2Shape_SetFriction(m_ShapeId, _Friction);
	
}

