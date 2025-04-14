#include "pch.h"
#include "GCollisionManager.h"

void GCollisionManager::Init()
{
	b2WorldDef WorldDef = b2DefaultWorldDef();
	WorldDef.gravity = { 0.0f, -10.0f };

	m_WorldId = b2CreateWorld(&WorldDef);

	m_SubStepCount = 4;
}