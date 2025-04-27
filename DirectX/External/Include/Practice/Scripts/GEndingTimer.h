#pragma once
#include "GEndingTrigger.h"

class GEndingTimer :
    public GEndingTrigger
{
private:
    class GPlayer* m_Player;
    float m_Duration;
    float m_Timer;
    
    bool m_SucessEnd;

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(class GCollider2D* _Other) override ;
    virtual void OnOverlapExit(class GCollider2D* _Other) override;


    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GEndingTimer)

public:
    GEndingTimer();
    GEndingTimer(const GEndingTimer& _Origin);
    ~GEndingTimer();
};