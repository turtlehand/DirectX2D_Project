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

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GItem);

public:
    GItem();
    ~GItem();
};

