#pragma once
#include "AssetUI.h"
class MeshUI :
    public AssetUI
{
public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    MeshUI();
    ~MeshUI();
};

