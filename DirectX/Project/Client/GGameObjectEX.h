#pragma once
#include <Engine/GGameObject.h>

class GGameObjectEX :
    public GGameObject
{
public:
    void FinalUpdate();

public:
    CLONE(GGameObjectEX)

public:
    GGameObjectEX();
    ~GGameObjectEX();
};

