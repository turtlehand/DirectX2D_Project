#include "pch.h"
#include "GTransform.h"

#include "GDevice.h"
#include "GConstBuffer.h"
#include "GCamera.h"

GTransform::GTransform() :
	GComponent(COMPONENT_TYPE::TRANSFORM),
	m_RelativePosition(0.f, 0.f, 0.f),
	m_RelativeScale(1.f, 1.f, 1.f),
	m_RelativeRotation(0.f, 0.f, 0.f)
{
	m_RelativeDir[0] = Vector3(1.f, 0.f, 0.f);
	m_RelativeDir[1] = Vector3(0.f, 1.f, 0.f);
	m_RelativeDir[2] = Vector3(0.f, 0.f, 1.f);
}

GTransform::~GTransform()
{

}

void GTransform::SetRelativeRotation(Vector3 _Rotation)
{
	m_RelativeRotation = _Rotation * (XM_PI / 180.f);
}

void GTransform::SetRelativeRotation(float _x, float _y, float _z)
{
	m_RelativeRotation = Vector3(_x, _y, _z) * (XM_PI / 180.f);
}

Vector3 GTransform::GetWorldScale() const
{
	GGameObject* pParent = GameObject()->GetParent();
	if (!pParent)
		return m_RelativeScale;

	return pParent->Transform()->GetWorldScale() * m_RelativeScale;
}

Vector3 GTransform::GetWorldRotation() const
{
	GGameObject* pParent = GameObject()->GetParent();
	if (!pParent)
		return m_RelativeRotation * (180.f / XM_PI);

	return pParent->Transform()->GetWorldRotation() + m_RelativeRotation * (180.f / XM_PI);
}

void GTransform::FinalUpdate()
{
	SetMatrix();

	if (!GameObject()->Camera())
	{
		DrawDebugLine(Vector4(1.f, 0.f, 0.f, 1.f), GetWorldPos(), m_RelativeDir[0], 25.f);
		DrawDebugLine(Vector4(0.f, 1.f, 0.f, 1.f), GetWorldPos(), m_RelativeDir[1], 25.f);
		DrawDebugLine(Vector4(0.f, 0.f, 1.f, 1.f), GetWorldPos(), m_RelativeDir[2], 25.f);
	}
	
}

void GTransform::SetMatrix()
{
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	Matrix matRotation = XMMatrixRotationX(m_RelativeRotation.x) * XMMatrixRotationY(m_RelativeRotation.y) * XMMatrixRotationZ(m_RelativeRotation.z);

	Matrix matTranslation = XMMatrixTranslation(m_RelativePosition.x, m_RelativePosition.y, m_RelativePosition.z);

	// 크기 -> 회전 -> 이동
	m_matWorld = matScale * matRotation * matTranslation;

	// 방향 벡터 설정
	m_RelativeDir[0] = Vector3(1.f, 0.f, 0.f);
	m_RelativeDir[1] = Vector3(0.f, 1.f, 0.f);
	m_RelativeDir[2] = Vector3(0.f, 0.f, 1.f);

	for (int i = 0; i < (int)DIRECTION_TYPE::END; ++i)
	{
		m_WorldDir[i] = m_RelativeDir[i] = XMVector3TransformNormal(m_RelativeDir[i], matRotation);
	}

	// 부모 오브젝트가 있는 경우, 부모의 월드행렬을 누적시킨다.
	if (GameObject()->GetParent())
	{
		// 부모의 월드행렬을 곱하여 월드행렬을 설정한다.
		// 크기 -> 회전 -> 이동 -> 부모
		m_matWorld *= GameObject()->GetParent()->Transform()->GetWorldMat();

		m_WorldDir[0] = Vector3(1.f, 0.f, 0.f);
		m_WorldDir[1] = Vector3(0.f, 1.f, 0.f);
		m_WorldDir[2] = Vector3(0.f, 0.f, 1.f);

		for (int i = 0; i < (int)DIRECTION_TYPE::END; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], m_matWorld);
		}
	}
}

void GTransform::Binding()
{
	// 오브젝트의 위치를 상수버퍼로 이동시킨다.
	GConstBuffer* pTransformBuffer = GDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	
	g_Trans.matWorld = m_matWorld;
	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

	pTransformBuffer->SetData(&g_Trans, sizeof(tTransform));
	pTransformBuffer->Binding();
}

void GTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_RelativePosition, sizeof(Vector3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vector3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vector3), 1, _File);
}

void GTransform::LoadFromFile(FILE* _File)
{
	fread(&m_RelativePosition, sizeof(Vector3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vector3), 1, _File);
	fread(&m_RelativeRotation, sizeof(Vector3), 1, _File);
}


