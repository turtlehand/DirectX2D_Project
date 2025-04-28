#pragma once
#include <Engine/GScript.h>
#include <Engine/GSound.h>

class GCameraLimit :
    public GScript
{
private:
    GGameObject* m_Target;
    class GCameraMove* m_MainCamera;
    bool m_TargetHere;

    float m_CameraScaleX;
    Ptr<GSound> m_BGM;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GCameraLimit)

public:
    GCameraLimit();
    ~GCameraLimit();
};
