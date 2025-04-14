#pragma once
#include <Engine/GScript.h>
#include "GObjectBasic.h"

class GWallChecker :
    public GScript
{
private:
    GObjectBasic* m_Owner;


public:
    virtual void Begin() override;
    virtual void Update() override;

    virtual void OnTriggerEnter(GCollider2D* _Other);
    virtual void OnTriggerStay(GCollider2D* _Other);
    virtual void OnTriggerExit(GCollider2D* _Other);

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GWallChecker);

public:
    GWallChecker();
    ~GWallChecker();
};