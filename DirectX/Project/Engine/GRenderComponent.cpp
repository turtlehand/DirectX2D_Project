#include "pch.h"
#include "GRenderComponent.h"

#include "GLevelManager.h"

#include "GAssetManager.h"

GRenderComponent::GRenderComponent(COMPONENT_TYPE _Type) :
	GComponent(_Type)
{

}

GRenderComponent::GRenderComponent(const GRenderComponent& _Origin)
	:GComponent(_Origin)
	,m_Mesh(_Origin.m_Mesh)
	,m_SharedMtrl(_Origin.m_SharedMtrl)
	,m_CurMtrl(m_SharedMtrl)
	,m_DynamicMtrl(nullptr)
{

}

GRenderComponent::~GRenderComponent()
{

}

void GRenderComponent::SetMaterial(Ptr<GMaterial> _Material)
{
	m_CurMtrl = m_SharedMtrl = _Material;
}

Ptr<GMaterial> GRenderComponent::GetDynamicMaterial()
{
	// 현재 레벨의  상태가 Play 모드인지 확인
	assert(LEVEL_STATE::PLAY == GLevelManager::GetInst()->GetCurrentLevelState());

	if (nullptr == m_DynamicMtrl)
		m_DynamicMtrl = m_CurMtrl->Clone();

	m_CurMtrl = m_DynamicMtrl;
	return m_DynamicMtrl;
}

void GRenderComponent::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMtrl, _File);
	SaveAssetRef(m_CurMtrl, _File);
}

void GRenderComponent::LoadFromFile(FILE* _File)
{
	m_Mesh = LoadAssetRef<GMesh>(_File);
	m_SharedMtrl = LoadAssetRef<GMaterial>(_File);
	m_CurMtrl = LoadAssetRef<GMaterial>(_File);
}