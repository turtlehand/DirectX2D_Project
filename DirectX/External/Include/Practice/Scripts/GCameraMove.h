#pragma once
#include <Engine/GScript.h>

class GCameraMove :
    public GScript
{
private:
    float   m_CamSpeed;

public:
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

