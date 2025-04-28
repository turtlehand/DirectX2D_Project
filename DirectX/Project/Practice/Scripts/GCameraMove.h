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

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
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

