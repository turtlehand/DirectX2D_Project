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

    virtual void OnTriggerEnter(GCollider2D* _Other);
    virtual void OnTriggerStay(GCollider2D* _Other);
    virtual void OnTriggerExit(GCollider2D* _Other);

private:
    void ChangeState();

public:
    GPlayerFlinchState();
    ~GPlayerFlinchState();
};