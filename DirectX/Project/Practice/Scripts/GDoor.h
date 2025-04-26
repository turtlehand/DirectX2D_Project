#pragma once
#include <Engine/GScript.h>
#include "GInteractable.h"

class GDoor :
    public GInteractable
{
private:
    bool m_StartInteract;
    Vector3 m_StartPos;
    Vector3 m_EndPos;
    
    float m_Duration;
    float m_Timer;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    virtual void InteractEnter();

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);
public:
    CLONE(GDoor);

public:
    GDoor();
    ~GDoor();
};

