#include "pch.h"
#include "GPlatform.h"

GPlatform::GPlatform()
	:GScript(PLATFORM)
	,m_PlatformType(PLATFORM_TYPE::DEFAULT)
{
}

GPlatform::~GPlatform()
{
}

void GPlatform::Init()
{
	ADD_INT("Platform Type", &m_PlatformType)
}

void GPlatform::Update()
{
}



void GPlatform::SaveToFile(FILE* _File)
{
}

void GPlatform::LoadFromFile(FILE* _File)
{
}


