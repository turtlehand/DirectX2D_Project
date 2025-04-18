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
    virtual void GroundEnter();
    virtual void GroundExit();
    virtual void CeilingEnter();
    virtual void CeilingExit();

public:
    virtual void Update() {};

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

public:
    CLONE(GObjectBasic);

public:
    GObjectBasic(SCRIPT_TYPE _Type = OBJECTBASIC);
    ~GObjectBasic();

    friend class GGroundChecker;
    friend class GWallChecker;
    friend class GCeilingChecker;
};

