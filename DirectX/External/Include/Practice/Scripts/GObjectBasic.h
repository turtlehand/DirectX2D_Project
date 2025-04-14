#pragma once
#include <Engine/GScript.h>

class GObjectBasic :
    public GScript
{
protected:
    int m_IsGround;
    int m_IsRightWall;
    int m_IsLeftWall;

public:
    GObjectBasic(SCRIPT_TYPE _Type);
    ~GObjectBasic();

    friend class GGroundChecker;
    friend class GWallChecker;
};

