#pragma once
#include "ComponentUI.h"
class Light2DUI :
    public ComponentUI
{

public:
    virtual void Render_UI() override;

public:
    Light2DUI();
    ~Light2DUI();
};
