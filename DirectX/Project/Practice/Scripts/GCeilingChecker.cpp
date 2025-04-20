#include "pch.h"
#include "GCeilingChecker.h"

#include <Engine/components.h>

GCeilingChecker::GCeilingChecker()
	:GScript(CEILINGCHECKER)
{
}

GCeilingChecker::~GCeilingChecker()
{
}


void GCeilingChecker::Begin()
{
	GGameObject* ParentObject = GameObject();
	while (true)
	{
		ParentObject = ParentObject->GetParent();
		assert(ParentObject != nullptr);

		m_Owner = ParentObject->GetComponent<GObjectBasic>();
		if (m_Owner != nullptr)
			return;
	}
}

void GCeilingChecker::Update()
{
}

void GCeilingChecker::OnOverlapEnter(GCollider2D* _Other)
{

	if (m_Owner == nullptr)
		return;

	if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
		return;

	++m_Owner->m_IsCeiling;

	m_Owner->CeilingEnter();
}

void GCeilingChecker::OnOverlapStay(GCollider2D* _Other)
{
}

void GCeilingChecker::OnOverlapExit(GCollider2D* _Other)
{
	if (m_Owner == nullptr)
		return;

	if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
		return;

	--m_Owner->m_IsCeiling;

	m_Owner->CeilingExit();
}
