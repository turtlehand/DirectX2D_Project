#include "pch.h"
#include "Missile.h"

#include <Engine\components.h>
#include <Engine\GTimeManager.h>

Missile::Missile()
	:GScript(MISSILE)
{
}

Missile::~Missile()
{
}

void Missile::Update()
{
	Vector3 vPos = Transform()->GetRelativePos();

	vPos += Vector3(0.f, 1.f, 0.f) * m_Speed * DT;

	Transform()->SetRelativePos(vPos);
}

void Missile::OnTriggerEnter(GCollider2D* _Other)
{

}

void Missile::OnTriggerStay(GCollider2D* _Other)
{

}

void Missile::OnTriggerExit(GCollider2D* _Other)
{

}

void Missile::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void Missile::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}
