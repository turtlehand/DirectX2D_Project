#include "pch.h"
#include "Monster.h"

#include <Engine\components.h>

Monster::Monster()
	:GScript(MONSTER)
{

}

Monster::~Monster()
{

}

void Monster::Update()
{

}

void Monster::OnTriggerEnter(GCollider2D* _Other)
{
	if (_Other->GameObject()->GetName() == L"Missile")
	{
		GameObject()->Destroy();
	}
}

void Monster::OnTriggerStay(GCollider2D* _Other)
{

}

void Monster::OnTriggerExit(GCollider2D* _Other)
{

}

void Monster::SaveToFile(FILE* _File)
{
}

void Monster::LoadFromFile(FILE* _File)
{
}


