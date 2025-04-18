#pragma once
#include <Engine/GScript.h>
#include "GState.h"

class GMinionWalkState :
    public GState
{
private:
    class GMinion* m_Minion;
    class GRigidBody2D* m_MinionRigid;

public:
    virtual void Awake();
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    GMinionWalkState();
    ~GMinionWalkState();
};
