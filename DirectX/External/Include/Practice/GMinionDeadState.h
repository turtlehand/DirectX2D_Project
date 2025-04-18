#pragma once
#include "GState.h"

class GMinionDeadState :
    public GState
{
private:
    class GMinion* m_Minion;

public:
    virtual void Awake();
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    GMinionDeadState();
    ~GMinionDeadState();
};