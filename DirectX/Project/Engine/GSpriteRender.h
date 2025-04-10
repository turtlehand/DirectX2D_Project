#pragma once
#include "GRenderComponent.h"
#include "GSprite.h"

class GSpriteRender :
    public GRenderComponent
{
private:
    Ptr<GSprite> m_Sprite;
    Vector4 m_Color;

public:
    void SetSprite(Ptr<GSprite> _Sprite) { m_Sprite = _Sprite; }
    Ptr<GSprite> GetSprite() { return m_Sprite; }

    void SetColor(const Vector4& _Color) { m_Color = _Color; }
    const Vector4& GetColor() { return m_Color; }

private:
    void CreateSpriteMaterial();

public:
    virtual void FinalUpdate() override;
    virtual void Render() override;

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GSpriteRender);
    GSpriteRender();
    ~GSpriteRender();

};

