#pragma once
#include "GFSM.h"
#include <Engine/GCollider2D.h>

class GCollider2D;

// 다중 상속 용
class GState
{
private:
    wstring m_Name;
    GFSM* m_Owner;

private:
    void SetName(const wstring& _Name) { m_Name = _Name; }

public:
    const wstring& GetName() { return m_Name; }
    GFSM* GetFSM() { return m_Owner; }

public:
    virtual void Awake() {};
    virtual void Enter() {};
    virtual void Enter(DWORD_PTR _Param1) {};
    virtual void Enter(DWORD_PTR _Param1, DWORD_PTR _Param2) {};
    virtual void Tick() = 0;
    virtual void Exit() = 0;

    virtual void OnTriggerEnter(GCollider2D* _Other) {};
    virtual void OnTriggerStay(GCollider2D* _Other) {};
    virtual void OnTriggerExit(GCollider2D* _Other) {};
public:
    GState();
    virtual ~GState();

    friend class GFSM;
};

