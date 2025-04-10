#pragma once

#include <Engine/GScript.h>

class GCamera;

class EditorCameraScript :
    public GScript
{
private:
    float   m_CamSpeed;
    GCamera* m_Cam;

public:
    virtual void Update() override;

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File) {};
    virtual void LoadFromFile(FILE* _File) {};

private:
    void Move_Perspective();
    void Move_OrthoGraphic();

public:
    CLONE(EditorCameraScript)

public:
    EditorCameraScript();
    ~EditorCameraScript();
};

