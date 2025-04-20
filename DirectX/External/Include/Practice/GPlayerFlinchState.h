#pragma once
#include "GState.h"

class GPlayer;

class GPlayerFlinchState :
    public GState
{
private:
    GPlayer* m_Player;
    GRigidBody2D* m_PlayerRigid;
    float m_Timer;

public:
    virtual void Awake();
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

    virtual void OnOverlapEnter(GCollider2D* _Other);
    virtual void OnOverlapStay(GCollider2D* _Other);
    virtual void OnOverlapExit(GCollider2D* _Other);

private:
    void ChangeState();

public:
    GPlayerFlinchState();
    ~GPlayerFlinchState();
};