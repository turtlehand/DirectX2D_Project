#pragma once
#include "GState.h"
class GPlayerNoneState :
    public GState
{
private:
    class GPlayer* m_Player;

public:
    virtual void Awake();
    virtual void Enter(DWORD_PTR _FlipBook);
    virtual void Tick();
    virtual void Exit();

    virtual void OnOverlapEnter(GCollider2D* _Other);
    virtual void OnOverlapStay(GCollider2D* _Other);
    virtual void OnOverlapExit(GCollider2D* _Other);

public:
    GPlayerNoneState();
    ~GPlayerNoneState();
};

