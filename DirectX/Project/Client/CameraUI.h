#pragma once
#include "ComponentUI.h"

class CameraUI :
    public ComponentUI
{

public:
    virtual void Render_UI() override;

public:
    CameraUI();
    ~CameraUI();
};

