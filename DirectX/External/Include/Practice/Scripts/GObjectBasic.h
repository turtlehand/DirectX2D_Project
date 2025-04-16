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

public:
    virtual void GroundEnter() {}
    virtual void CeilingEnter() {}

public:
    GObjectBasic(SCRIPT_TYPE _Type);
    ~GObjectBasic();

    friend class GGroundChecker;
    friend class GWallChecker;
    friend class GCeilingChecker;
};

