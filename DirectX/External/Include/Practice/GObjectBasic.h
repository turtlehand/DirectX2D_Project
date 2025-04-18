#pragma once
#include <Engine/GScript.h>
#include "GPlatform.h"


class GObjectBasic :
    public GScript
{
protected:
    vector<GGameObject*> m_vecPlatform;
    int m_IsGround;
    int m_IsCeiling;

    float	m_GravityScale;

    Vector2 m_FlinchForce;
    int m_MaxHP;
    int m_HP;

public:
    void SetMoveDirection(int _Direction);

    virtual void GroundEnter();
    virtual void GroundExit();
    virtual void CeilingEnter();
    virtual void CeilingExit();

public:
    GObjectBasic(SCRIPT_TYPE _Type);
    GObjectBasic(const GObjectBasic& _Origin);
    ~GObjectBasic();

    friend class GGroundChecker;
    friend class GWallChecker;
    friend class GCeilingChecker;
};

