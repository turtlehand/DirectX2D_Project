#pragma once
#include <Engine/GScript.h>

class GObjectBasic :
    public GScript
{
protected:
    int m_IsGround;
    int m_IsRightWall;
    int m_IsLeftWall;
    int m_IsCeiling;

    float			m_GravityScale;

public:
    virtual void GroundEnter();
    virtual void GroundExit();
    virtual void CeilingEnter();
    virtual void CeilingExit();
public:
    GObjectBasic(SCRIPT_TYPE _Type);
    ~GObjectBasic();

    friend class GGroundChecker;
    friend class GWallChecker;
    friend class GCeilingChecker;
};

