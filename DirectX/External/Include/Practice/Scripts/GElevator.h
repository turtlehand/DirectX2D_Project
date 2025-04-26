#pragma once
#include "GInteractable.h"

class GElevator :
    public GInteractable
{
private:
    bool m_StartMove;
    Vector3 m_TopPos;
    Vector3 m_BottomPos;
    
    int m_Direction;

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
    CLONE(GElevator);

public:
    GElevator();
    GElevator(const GElevator& _Origin);
    ~GElevator();
};