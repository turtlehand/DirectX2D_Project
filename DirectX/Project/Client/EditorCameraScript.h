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

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
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

