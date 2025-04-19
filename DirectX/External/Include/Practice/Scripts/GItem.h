#pragma once
#include <Engine/GScript.h>
#include <Engine/GSprite.h>

class GItem :
    public GScript
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

