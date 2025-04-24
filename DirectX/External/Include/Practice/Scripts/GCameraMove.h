#pragma once
#include <Engine/GScript.h>

class GCameraMove :
    public GScript
{
private:
    GGameObject* m_Target;
    float   m_CamSpeed;
    Vector3   m_Offset;

public:
    virtual void Begin() override;
    virtual void Update() override;

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(GCameraMove)

public:
    GCameraMove();
    ~GCameraMove();
};

