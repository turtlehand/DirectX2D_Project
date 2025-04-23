#pragma once
#include <Engine/GScript.h>
#include "GGameManager.h"

class GEndingTrigger :
    public GScript
{
private:
    ENDING_TYPE m_EndingType;

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(class GCollider2D* _Other) override;


    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GEndingTrigger)

public:
    GEndingTrigger();
    ~GEndingTrigger();
};

