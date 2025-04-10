#pragma once
#include "GRenderComponent.h"

#include "GMesh.h"
#include "GGraphicShader.h"

class GMeshRender :
    public GRenderComponent
{
private:
    Vector4 m_Color;

public:
    void SetColor(const Vector4& _Color) { m_Color = _Color; }
    const Vector4& GetColor() { return m_Color; }

public:
    virtual void FinalUpdate() override;
    virtual void Render() override;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GMeshRender);

public:
    GMeshRender();
    ~GMeshRender();
};

