#pragma once
#include "GState.h"

class GPlayer;
class GRigidBody2D;
class GGameObject;
class GSound;

class GPlayerUseItemState :
    public GState
{
private:
    GPlayer* m_Player;
    GRigidBody2D* m_PlayerRigid;

    bool m_IsHugSucess;

    //Ptr<GSound> m_SwordSound;
    Ptr<GSound> m_HookSound;
    Ptr<GSound> m_ShovelSound;
    Ptr<GSound> m_HugSound;
    Ptr<GSound> m_HugSound_None;


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
    GPlayerUseItemState();
    ~GPlayerUseItemState();
};
