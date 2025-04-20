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

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(GGameManagerScript)
public:
    GGameManagerScript();
    ~GGameManagerScript();

    friend class GGameManager;
};