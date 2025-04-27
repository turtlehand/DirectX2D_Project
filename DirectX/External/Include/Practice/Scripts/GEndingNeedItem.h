#pragma once
#include "GEndingTrigger.h"

class GEndingNeedItem :
    public GEndingTrigger
{
private:
    PLAYER_ITEM m_Item;

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(class GCollider2D* _Other) override;


    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GEndingNeedItem)

public:
    GEndingNeedItem();
    GEndingNeedItem(const GEndingNeedItem& _Origin);
    ~GEndingNeedItem();
};

