#pragma once
#include <Engine/GScript.h>

class GSprite;

class GSwitch :
    public GScript
{
private:
    int m_IsPressed;
    Ptr<GSprite> m_ButtonSprite;
    Ptr<GSprite> m_PressedSprite;

    string m_ObjectName[3];
    bool m_SucessFindObject[3];
    GGameObject* m_Object[3];

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(GCollider2D* _Other) override;
    virtual void OnOverlapExit(GCollider2D* _Other) override;

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GSwitch);

public:
    GSwitch();
    GSwitch(const GSwitch& _Origin);
    ~GSwitch();
};

