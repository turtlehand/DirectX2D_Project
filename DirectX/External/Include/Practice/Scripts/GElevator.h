#pragma once
#include "GInteractable.h"

class GElevator :
    public GInteractable
{
private:
    bool m_StartMove;       // ���� �����̰� �ִ��� ����
    Vector3 m_TopPos;       // ����
    Vector3 m_BottomPos;    // �Ʒ���
    
    int m_Direction;        // ���� �����̰� �ִ� ����. Top���� �ö󰡰� �ִٸ� 1, Bottom���� �������� �ִٸ� -1, �����̰� ���� �ʴٸ� 0

    float m_Duration;       // Duration �� ���� �����δ�.
    float m_Timer;          // �����̴� �ð� ī����

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