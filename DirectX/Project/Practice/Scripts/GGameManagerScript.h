#pragma once
#include <Engine/GScript.h>
#include "GGameManager.h"


class GGameManagerScript :
    public GScript
{
public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(GGameManagerScript)
public:
    GGameManagerScript();
    ~GGameManagerScript();

    friend class GGameManager;
};