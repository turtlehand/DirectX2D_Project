#include "pch.h"
#include "GComponent.h"

#include "GLevelManager.h"
#include "GLevel.h"
#include "GTaskManager.h"

GComponent::GComponent(COMPONENT_TYPE _Type) :
	m_Type(_Type),
	m_GameObject(nullptr)
{
}

GComponent::GComponent(const GComponent& _Origin)
	: GEntity(_Origin)
	, m_Type(_Origin.m_Type)
	, m_GameObject(nullptr)
{

}

GComponent::~GComponent()
{

}

void GComponent::Destroy()
{
	tTask task = { TASK_TYPE::DESTROY_COMPONENT, (DWORD_PTR)GameObject(), (DWORD_PTR)this };

	GTaskManager::GetInst()->AddTask(task);
}
