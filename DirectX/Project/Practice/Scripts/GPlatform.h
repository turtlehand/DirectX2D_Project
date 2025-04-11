#pragma once
#include <Engine/GScript.h>

enum class PLATFORM_TYPE
{
    DEFAULT,
    WATER,
    LAVA,
    SOFT,
    WOOD
};

class GPlatform :
    public GScript
{
private:
    PLATFORM_TYPE m_PlatformType;

public:
    PLATFORM_TYPE GetPlatformType() { return m_PlatformType; }

public:
    virtual void Init() override;
    virtual void Update() override;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GPlatform);

public:
    GPlatform();
    ~GPlatform();
};

