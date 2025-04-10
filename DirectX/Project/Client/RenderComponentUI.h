#pragma once
#include "ComponentUI.h"
class RenderComponentUI :
    public ComponentUI
{
private:

public:
    void SelectMesh(DWORD_PTR _UI);
    void SelectMaterial(DWORD_PTR _UI);

public:
    virtual void Render_UI() override;

private:
    void MeshUI();
    void MateiralUI();
    

public:
    RenderComponentUI(COMPONENT_TYPE _Type);
    ~RenderComponentUI();
};

