#pragma once
#include <Engine/GScript.h>
#include "GObjectBasic.h"

class GGroundChecker :
    public GScript
{
private:
    GObjectBasic* m_Owner;

public:
    virtual void Begin() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(GCollider2D* _Other);
    virtual void OnOverlapStay(GCollider2D* _Other);
    virtual void OnOverlapExit(GCollider2D* _Other);

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}
public:
    CLONE(GGroundChecker);

public:
    GGroundChecker();
    ~GGroundChecker();
};

