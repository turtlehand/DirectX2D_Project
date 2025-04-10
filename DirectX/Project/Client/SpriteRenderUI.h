#pragma once
#include "RenderComponentUI.h"

class GSpriteRender;

class SpriteRenderUI :
    public RenderComponentUI
{
    GSpriteRender* m_SpriteRender;

public:
    void SelectSprite(DWORD_PTR _UI);

public:
    virtual void Render_UI() override;

public:
    SpriteRenderUI();
    ~SpriteRenderUI();
};

