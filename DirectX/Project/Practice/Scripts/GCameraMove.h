#pragma once
#include <Engine/GScript.h>

class GCameraMove :
    public GScript
{
private:
    GGameObject* m_Target;
    float   m_CamSpeed;
    Vector3   m_Offset;

    Vector3     m_Center;
    Vector3     m_PostCenter;

    Vector3     m_MapSize;
    Vector3     m_PostMapSize;

    float       m_PostCameraSize;

public:
    void SetCenter(Vector3 _Center) { m_PostCenter = _Center; }
    void SetMapSize(Vector3 _MapSize) { m_PostMapSize = _MapSize; }
    void SetPostCameraSize(float _Size) { m_PostCameraSize = _Size; }

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

private:
    void LimitCameraArea();

public:
    CLONE(GCameraMove)

public:
    GCameraMove();
    ~GCameraMove();
};

