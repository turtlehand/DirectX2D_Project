#pragma once
#include <Engine/GScript.h>
#include "GGameManager.h"

class GEndingTrigger :
    public GScript
{
protected:
    ENDING_TYPE m_EndingType;

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(class GCollider2D* _Other) override;


    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GEndingTrigger)

public:
    GEndingTrigger(SCRIPT_TYPE _Type = ENDINGTRIGGER);
    GEndingTrigger(const GEndingTrigger& _Origin, SCRIPT_TYPE _Type = ENDINGTRIGGER);
    ~GEndingTrigger();
};

