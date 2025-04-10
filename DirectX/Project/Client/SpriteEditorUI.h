#pragma once
#include "EditorUI.h"

class GTexture;
class GSprite;

class SpriteEditorUI :
    public EditorUI
{
private:
    Ptr<GTexture> m_Atlas;
    Vector2 m_RowCol;
    Vector2 m_LeftTop;
    Vector2 m_Slice;
    Vector2 m_Background;
    Vector2 m_Gap;
    
    string m_SpriteName;

private:
    void SelectTexture(DWORD_PTR _UI);

    void Reset();
    void InputSpriteValue();
    void PreviewSprite();
    void CreateSprite();

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    SpriteEditorUI();
    ~SpriteEditorUI();
};

