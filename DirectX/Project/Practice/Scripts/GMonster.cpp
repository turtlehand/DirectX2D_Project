#include "pch.h"
#include "GMonster.h"

#include <Engine\components.h>

GMonster::GMonster()
	:GScript(MONSTER)
{

}

GMonster::~GMonster()
{

}

void GMonster::Update()
{

}

void GMonster::OnTriggerEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetName() == L"Missile")
	{
		GameObject()->Destroy();
	}
}

void GMonster::OnTriggerStay(GCollider2D* _Other)
{

}

void GMonster::OnTriggerExit(GCollider2D* _Other)
{

}

void GMonster::SaveToFile(FILE* _File)
{
}

void GMonster::LoadFromFile(FILE* _File)
{
}


