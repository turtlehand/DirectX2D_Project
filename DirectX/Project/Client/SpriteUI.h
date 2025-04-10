#pragma once
#include "AssetUI.h"

class GSprite;

class SpriteUI :
    public AssetUI
{
private:
    Ptr<GSprite> m_Sprite;

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

private:
    void Sprite();
    void Texture();

    void SelectTexture(DWORD_PTR _UI);

public:
    SpriteUI();
    ~SpriteUI();
};

