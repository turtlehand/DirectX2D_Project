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

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GCameraLimit)

public:
    GCameraLimit();
    ~GCameraLimit();
};
