#include "pch.h"
#include "GLord.h"

#include <Engine/GScript.h>

void GLord::Init()
{
}

void GLord::Begin()
{
}

void GLord::Update()
{
}

void GLord::OnOverlapEnter(GCollider2D* _Other)
{
}

void GLord::SaveToFile(FILE* _File)
{
}

void GLord::LoadFromFile(FILE* _File)
{
}

GLord::GLord()
	: GObjectBasic(LORD)
{
}

GLord::GLord(const GLord& _Origin)
	: GObjectBasic(LORD)
	, m_FSM(nullptr)
{

}

GLord::~GLord()
{
}
