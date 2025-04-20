#include "pch.h"
#include "GGameManagerScript.h"

#include "GGameManager.h"

GGameManagerScript::GGameManagerScript()
	:GScript(GAMEMANAGERSCRIPT)
{
}

GGameManagerScript::~GGameManagerScript()
{
}

void GGameManagerScript::Init()
{
	GGameManager::GetInst()->Init();
}

void GGameManagerScript::Begin()
{
	GGameManager::GetInst()->Begin();
}

void GGameManagerScript::Update()
{
	GGameManager::GetInst()->Progress();
}

void GGameManagerScript::SaveToFile(FILE* _File)
{
}

void GGameManagerScript::LoadFromFile(FILE* _File)
{
}


