#pragma once
#include "RenderComponentUI.h"

class GFlipbookRender;

class FlipbookRenderUI :
    public RenderComponentUI
{
private:
    GFlipbookRender* m_FlipbookRender;
    int m_FlipbookIdx;
public:
    void SelectFlipbook(DWORD_PTR _UI);

public:
    virtual void Render_UI() override;

private:
    void FlipbookList();

public:
    FlipbookRenderUI();
    ~FlipbookRenderUI();
};

