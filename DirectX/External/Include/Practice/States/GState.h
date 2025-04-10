#pragma once
#include "GFSM.h"

// ���� ��� ��
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
    virtual void Enter() = 0;
    virtual void Tick() = 0;
    virtual void Exit() = 0;

public:
    GState();
    virtual ~GState();

    friend class GFSM;
};

