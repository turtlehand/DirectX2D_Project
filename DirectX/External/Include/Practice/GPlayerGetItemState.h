#pragma once
#include "GState.h"

class GPlayer;
class GRigidBody2D;
class GItem;

class GPlayerGetItemState :
    public GState
{
private:
    GPlayer* m_Player;
    GRigidBody2D* m_PlayerRigid;
    float m_Timer;

    GItem* m_Item;

public:
    virtual void Awake();
    virtual void Enter(DWORD_PTR _Item);
    virtual void Tick();
    virtual void Exit();

    virtual void OnOverlapEnter(GCollider2D* _Other);
    virtual void OnOverlapStay(GCollider2D* _Other);
    virtual void OnOverlapExit(GCollider2D* _Other);

public:
    GPlayerGetItemState();
    ~GPlayerGetItemState();
};
