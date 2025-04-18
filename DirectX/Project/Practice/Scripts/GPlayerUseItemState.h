#pragma once
#include "GState.h"

class GPlayer;
class GRigidBody2D;
class GGameObject;

class GPlayerUseItemState :
    public GScript, public GState
{
private:
    GPlayer* m_Player;
    GRigidBody2D* m_PlayerRigid;

    bool m_IsHugSucess;

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

    virtual void OnTriggerEnter(GCollider2D* _Other);

private:
    void ChangeState();

    void Enter_Hug();
    void Tick_Hug();
    void Exit_Hug();

    void Enter_Shovel();
    void Tick_Shovel();
    void Exit_Shovel();



    void Tick_Hook();

public:
    CLONE(GPlayerUseItemState)

public:
    GPlayerUseItemState();
    ~GPlayerUseItemState();
};
