#pragma once
#include <Engine/GScript.h>

class GEndingScene :
    public GScript
{
public:
    virtual void Init() override;
    virtual void Update() override;

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(GEndingScene)
public:
    GEndingScene();
    ~GEndingScene();

    friend class GGameManager;
};

