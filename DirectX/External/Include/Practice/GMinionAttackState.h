#pragma once
#include "GState.h"

class GMinionAttackState :
    public GState
{
private:
    class GMinion* m_Minion;

    class GGameObject* m_Sword;
    float m_Timer;

public:
    virtual void Awake();
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    GMinionAttackState();
    ~GMinionAttackState();
};
