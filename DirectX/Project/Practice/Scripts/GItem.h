#pragma once
#include "GObjectBasic.h"
#include <Engine/GSprite.h>

class GItem :
    public GObjectBasic
{
private:
    PLAYER_ITEM m_ItemType;
    static vector<string> ItemName;

public:
    PLAYER_ITEM GetItemType() { return  m_ItemType; }

public:
    virtual void Init();
    virtual void Update();

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GItem);

public:
    GItem();
    ~GItem();
};

