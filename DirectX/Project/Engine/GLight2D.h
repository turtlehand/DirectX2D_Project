#pragma once
#include "GComponent.h"
class GLight2D :
    public GComponent
{
private:
    tLight2DInfo m_Info;

public:
    void SetLightType(LIGHT_TYPE _Type) { m_Info.Type = _Type; }
    void SetLightColor(Vector3 _Color) { m_Info.Color = _Color; }
    void SetRadius(float _Radius) { m_Info.Radius = _Radius; }
    void SetDegree(float _Degree) { m_Info.Angle = _Degree * (XM_PI / 180.f); }

    const tLight2DInfo& GetInfo() { return m_Info; }

    LIGHT_TYPE GetLightType() { return m_Info.Type; }
    Vector3 GetLightColor() { return m_Info.Color; }
    float GetRadius() { return m_Info.Radius; }
    float GetDegree() { return m_Info.Angle * (180.f / XM_PI); }


public:
    virtual void FinalUpdate() override;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GLight2D);
    GLight2D();
    ~GLight2D();
};

