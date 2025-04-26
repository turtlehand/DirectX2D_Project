#pragma once
#include <Engine/GScript.h>
#include "GInteractable.h"

class GDoor :
    public GInteractable
{
private:
    bool m_StartInteract;
    Vector3 m_StartPos;
    Vector3 m_EndPos;
    
    float m_Duration;
    float m_Timer;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    virtual void InteractEnter();

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GDoor);

public:
    GDoor();
    ~GDoor();
};

