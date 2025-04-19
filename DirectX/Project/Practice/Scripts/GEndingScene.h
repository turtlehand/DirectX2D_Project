#pragma once
#include <Engine/GScript.h>

class GEndingScene :
    public GScript
{
public:
    virtual void Init() override;
    virtual void Update() override;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(GEndingScene)
public:
    GEndingScene();
    ~GEndingScene();

    friend class GGameManager;
};

