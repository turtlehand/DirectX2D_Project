#pragma once
#include <Engine/GScript.h>
#include <Engine/GPrefab.h>

enum class PLAYER_STATE
{
    DEFAULT,
    WALK
};

class GFSM;

class GPlayer :
    public GScript
{
private:
    PLAYER_STATE m_PlayerState;
    GFSM* m_FSM;

    float m_Speed;

public:
    GFSM* GetFSM() { return m_FSM; }

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    virtual void OnTriggerEnter(GCollider2D* _Other);
    virtual void OnTriggerStay(GCollider2D* _Other);
    virtual void OnTriggerExit(GCollider2D* _Other);

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GPlayer);
public:
    GPlayer();
    ~GPlayer();

    friend class GPlayerDefaultState;
    friend class GPlayerWalkState;
};

