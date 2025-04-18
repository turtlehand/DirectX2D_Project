#include "pch.h"
#include "GGroundChecker.h"

#include <Engine/components.h>

GGroundChecker::GGroundChecker()
	:GScript(GROUNDCHECKER)
	, m_Owner(nullptr)
{
}

GGroundChecker::~GGroundChecker()
{
}

void GGroundChecker::Begin()
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

void GGroundChecker::Update()
{
}

void GGroundChecker::OnTriggerEnter(GCollider2D* _Other)
{
	
	if (m_Owner == nullptr)
		return;

	if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
	return;

	++m_Owner->m_IsGround;
	m_Owner->m_vecPlatform.push_back(_Other->GameObject());

	m_Owner->GroundEnter();

	Vector3 OtherPos = _Other->Transform()->GetWorldPos();
	Vector3 OtherScale = _Other->Transform()->GetWorldScale();
	Vector3 ThisPos = GameObject()->Transform()->GetWorldPos();
	Vector3 ThisScale = GameObject()->Transform()->GetWorldScale();
}

void GGroundChecker::OnTriggerStay(GCollider2D* _Other)
{

}

void GGroundChecker::OnTriggerExit(GCollider2D* _Other)
{
	if (m_Owner == nullptr)
		return;

	if (_Other->GameObject()->GetLayer() != (int)LAYER_TYPE::PLATFORM)
		return;

	--m_Owner->m_IsGround;
	
	vector<GGameObject*>::iterator iter = std::find(m_Owner->m_vecPlatform.begin(), m_Owner->m_vecPlatform.end(),_Other->GameObject());
	m_Owner->m_vecPlatform.erase(iter);

	m_Owner->GroundExit();
}



