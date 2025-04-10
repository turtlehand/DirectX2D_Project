#include "pch.h"
#include "GEntity.h"

UINT GEntity::g_NextID = 0;

GEntity::GEntity() :
	m_ID(g_NextID++),
	m_Name()
{

}

GEntity::GEntity(const GEntity& _Other) :
	m_ID(g_NextID++),
	m_Name(_Other.m_Name)
{

}

GEntity::~GEntity()
{

}
