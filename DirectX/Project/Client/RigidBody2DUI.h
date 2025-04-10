#pragma once
#include "ComponentUI.h"
class RigidBody2DUI :
    public ComponentUI
{
public:
    virtual void Render_UI() override;

public:
    RigidBody2DUI();
    ~RigidBody2DUI();
};

