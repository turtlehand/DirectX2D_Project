#pragma once
#include "RenderComponentUI.h"

class GMeshRender;

class MeshRenderUI :
    public RenderComponentUI
{
private:
    GMeshRender* m_MeshRender;

public:
    virtual void Render_UI() override;

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

