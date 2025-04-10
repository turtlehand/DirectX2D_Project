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

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
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

