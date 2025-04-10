#pragma once
#include <Engine/GScript.h>
#include "GState.h"

class GPlayer;

class GPlayerDefaultState :
    public GScript , public GState
{
private:

    GPlayer* m_Player;

public:
    virtual void Update() {};

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    virtual void Awake();
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    CLONE(GPlayerDefaultState)

public:
    GPlayerDefaultState();
    ~GPlayerDefaultState();
};

