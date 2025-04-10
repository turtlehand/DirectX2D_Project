#pragma once
#include "GComputeShader.h"
#include "GTexture.h"

class GSetColorCS :
    public GComputeShader
{
private:
    Ptr<GTexture>   m_TargetTex;
    Vector4         m_Color;

public:
    void SetTargetTex(Ptr<GTexture> _Tex) { m_TargetTex = _Tex; }
    void SetColor(Vector4 _Color) { m_Color = _Color; }

    virtual int Binding() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;
public:
    GSetColorCS();
    ~GSetColorCS();
};

