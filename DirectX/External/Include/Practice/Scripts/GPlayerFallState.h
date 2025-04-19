#pragma once
#include <Engine/GScript.h>
#include "GState.h"

class GPlayer;

class GPlayerFallState :
    public GScript, public GState
{
private:
    GPlayer* m_Player;
    GRigidBody2D* m_PlayerRigid;

public:
    virtual void Update() {};

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

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
    CLONE(GPlayerFallState)

public:
    GPlayerFallState();
    ~GPlayerFallState();
};