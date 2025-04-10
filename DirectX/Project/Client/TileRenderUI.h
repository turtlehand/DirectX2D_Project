#pragma once
#include "RenderComponentUI.h"

#include <Engine/GTileRender.h>

class GTileRender;
class GSprite;
struct tTileInfo;

class TileRenderUI :
    public RenderComponentUI
{
private:
    GTileRender* m_TileRender;
    
    TILE_PALETTE_PARAM m_CurTP;         // 현재 선택된 TP
    tTileInfo m_DrawTile;              // 현재 선택된 TP에서 선택한 Tile

    Vector2 m_LeftTop;
    Vector2 m_Slice;

public:
    virtual void Render_UI() override;
    virtual void Update_UI() override;

    virtual void Activate() override;

private:
    void SelectTP(DWORD_PTR _UI);

    void TilePalette(TILE_PALETTE_PARAM _Param);

    void PreviewCurTile();
    void PreviewTiles();

public:
    TileRenderUI();
    ~TileRenderUI();
};
