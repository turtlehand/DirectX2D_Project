#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:
    TEX_PARAM m_TexParam;
public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

private:
    void Material();
    void Shader();
    void Scalar_Parameter();
    void Texture_Parameter();

private:
    void SelectShader(DWORD_PTR _Data);
    void SelectTexture(DWORD_PTR _UI);

public:
    MaterialUI();
    ~MaterialUI();
};

