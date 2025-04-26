#pragma once
#include <Engine/GScript.h>

class GInteractable :
	public GScript
{
public:
	bool m_SucessInteraction;

public:
	virtual void InteractEnter() {};
	virtual void InteractStay() {};
	virtual void InteractExit() {};

public:
	GInteractable(SCRIPT_TYPE _Type) : GScript(_Type) {};
	virtual ~GInteractable() {};
};

