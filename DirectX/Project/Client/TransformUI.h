#pragma once
#include "ComponentUI.h"
class TransformUI :
    public ComponentUI
{

public:
    virtual void Render_UI() override;

public:
    TransformUI();
    ~TransformUI();
};

