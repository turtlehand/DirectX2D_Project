#include "pch.h"
#include "GGameObjectEX.h"

#include <Engine/GComponent.h>

GGameObjectEX::GGameObjectEX()
	:GGameObject(-1)
{
}

GGameObjectEX::~GGameObjectEX()
{
}

void GGameObjectEX::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != GetComponent((COMPONENT_TYPE)i))
			GetComponent((COMPONENT_TYPE)i)->FinalUpdate();
	}

	// �ڽĵ� FinalUpdate �����ֱ�
	vector<GGameObject*>::const_iterator iter = GetChild().begin();
	for (; iter != GetChild().end(); ++iter )
	{
		(*iter)->FinalUpdate();

	}

}


