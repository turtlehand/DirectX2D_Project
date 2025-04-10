#include "pch.h"
#include "GScript.h"

#include "GTransform.h"
#include "GPrefab.h"

GScript::GScript(int _ScriptType) 
	: GComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{

}

GScript::GScript(const GScript& _Origin)
	: GComponent(_Origin)
	, m_ScriptType(_Origin.m_ScriptType)
{
}

GScript::~GScript()
{
}

void GScript::FinalUpdate()
{
}

void GScript::Instantiate(Ptr<GPrefab> _Prefab, Vector3 _WorldPos)
{
	assert(_Prefab.Get());

	GGameObject* InstantObject = _Prefab->Instantiate();
	InstantObject->Transform()->SetRelativePos(_WorldPos);
	SpawnGameObject(InstantObject);
}