#pragma once
#include "GInteractable.h"

class GElevator :
    public GInteractable
{
private:
    bool m_StartMove;       // 현재 움직이고 있는지 여부
    Vector3 m_TopPos;       // 위층
    Vector3 m_BottomPos;    // 아래층
    
    int m_Direction;        // 현재 움직이고 있는 방향. Top으로 올라가고 있다면 1, Bottom으로 내려가고 있다면 -1, 움직이고 있지 않다면 0

    float m_Duration;       // Duration 초 동안 움직인다.
    float m_Timer;          // 움직이는 시간 카운터

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