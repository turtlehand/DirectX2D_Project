#pragma once
#include <Engine/GScript.h>
#include "GState.h"

class GPlayer;
class GRigidBody2D;
class GSound;

class GPlayerWalkState :
    public GScript, public GState
{
private:
    GPlayer* m_Player;
    GRigidBody2D* m_PlayerRigid;

    Ptr<GSound> m_WalkSound;
    float m_WalkSound_Duration;
    float m_WalkSound_Timer;

    float m_JumpBonusTimer;

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
    CLONE(GPlayerWalkState)

public:
    GPlayerWalkState();
    ~GPlayerWalkState();
};
