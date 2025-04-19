#pragma once
#include <Engine/GScript.h>
#include "GObjectBasic.h"

class GGroundChecker :
    public GScript
{
private:
    GObjectBasic* m_Owner;

public:
    virtual void Begin() override;
    virtual void Update() override;

    virtual void OnOverlapEnter(GCollider2D* _Other);
    virtual void OnOverlapStay(GCollider2D* _Other);
    virtual void OnOverlapExit(GCollider2D* _Other);

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}
public:
    CLONE(GGroundChecker);

public:
    GGroundChecker();
    ~GGroundChecker();
};

