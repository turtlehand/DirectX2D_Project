#pragma once
#include "AssetUI.h"

#include <Engine/GTilePalette.h>

class TilePaletteUI :
    public AssetUI
{
private:
    Ptr<GTilePalette> m_TP;

    Ptr<GTexture> m_Atlas;
    Vector2 m_RowCol;
    Vector2 m_LeftTop;
    Vector2 m_Gap;
    Vector2 m_Slice;

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

private:
    void SelectTexture(DWORD_PTR _UI);
    void InputTileValue();
    void Tile();

public:
    TilePaletteUI();
    ~TilePaletteUI();
};
