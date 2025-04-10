#include "pch.h"
#include "GCollider2D.h"
#include "GScript.h"

#include "GTransform.h"

#include "GLevelManager.h"
#include "GLevel.h"
#include "GLayer.h"

GCollider2D::GCollider2D()
	: GComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Scale(Vector3(1.f,1.f,1.f))
	, m_Offset()
	, m_OverlapCount(0)
	, m_NotifyParent(true)
{
}

GCollider2D::GCollider2D(const GCollider2D& _Origin)
	: GComponent(_Origin)
	, m_Scale(_Origin.m_Scale)
	, m_Offset(_Origin.m_Offset)
	, m_OverlapCount(0)
	, m_NotifyParent(_Origin.m_NotifyParent)
{

}

GCollider2D::~GCollider2D()
{
}


void GCollider2D::OnTriggerEnter(GCollider2D* _Other)
{
	++m_OverlapCount;
	NotifyEnter(_Other);
}

void GCollider2D::OnTriggerStay(GCollider2D* _Other)
{
	NotifyStay(_Other);
}

void GCollider2D::OnTriggerExit(GCollider2D* _Other)
{
	--m_OverlapCount;
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

void GCollider2D::FinalUpdate()
{
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);

	m_matColWorld = matScale * matTranslation * Transform()->GetWorldMat();

	if (m_OverlapCount)
		DrawDebugRect(Vector4(1.f, 0.f, 0.f, 1.f), m_matColWorld);
	else
		DrawDebugRect(Vector4(0.f, 1.f, 0.f, 1.f), m_matColWorld);

	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(GameObject()->GetLayer())->RegisterCollider2D(this);
}

void GCollider2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Scale, sizeof(Vector3), 1, _File);
	fwrite(&m_Offset, sizeof(Vector3), 1, _File);
	fwrite(&m_NotifyParent, sizeof(bool), 1, _File);
}

void GCollider2D::LoadFromFile(FILE* _File)
{
	fread(&m_Scale, sizeof(Vector3), 1, _File);
	fread(&m_Offset, sizeof(Vector3), 1, _File);
	fread(&m_NotifyParent, sizeof(bool), 1, _File);
}