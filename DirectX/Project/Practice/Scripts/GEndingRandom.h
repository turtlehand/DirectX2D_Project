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


    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GEndingRandom)

public:
    GEndingRandom();
    GEndingRandom(const GEndingRandom& _Origin);
    ~GEndingRandom();
};