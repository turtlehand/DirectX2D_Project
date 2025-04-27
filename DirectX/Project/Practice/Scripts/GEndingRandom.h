#pragma once
#include "GEndingTrigger.h"

class GEndingRandom :
    public GEndingTrigger
{
private:
    float m_Prob;

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(class GCollider2D* _Other) override;


    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GEndingRandom)

public:
    GEndingRandom();
    GEndingRandom(const GEndingRandom& _Origin);
    ~GEndingRandom();
};