#pragma once
#include <Engine/GScript.h>
#include "GObjectBasic.h"

class GWallChecker :
    public GScript
{
private:
    GObjectBasic* m_Owner;


public:
    virtual void Begin() override;
    virtual void Update() override;

    virtual void OnTriggerEnter(GCollider2D* _Other);
    virtual void OnTriggerStay(GCollider2D* _Other);
    virtual void OnTriggerExit(GCollider2D* _Other);

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GWallChecker);

public:
    GWallChecker();
    ~GWallChecker();
};