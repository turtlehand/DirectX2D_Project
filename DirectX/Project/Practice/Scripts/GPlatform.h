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

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GPlatform);

public:
    GPlatform();
    ~GPlatform();
};

