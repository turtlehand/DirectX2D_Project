#pragma once
#include "GInteractable.h"

class GElevator :
    public GInteractable
{
private:
    bool m_StartMove;
    Vector3 m_TopPos;
    Vector3 m_BottomPos;
    
    int m_Direction;

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
    CLONE(GElevator);

public:
    GElevator();
    GElevator(const GElevator& _Origin);
    ~GElevator();
};