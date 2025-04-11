#include "pch.h"
#include "GState.h"

GState::GState()
	: m_Owner(nullptr)
{
	SetName(L"State");
}

GState::~GState()
{
}
