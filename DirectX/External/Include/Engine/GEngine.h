#pragma once
#include "GSingleton.h"

class GLevel;

class GEngine :
    public GSingleton<GEngine>
{
    SINGLE(GEngine);
private:
    HINSTANCE   m_hInst;          // 프로세스 주소
    HWND        m_hMainWnd;            // 메인 윈도우 핸들
    Vector2     m_vResolution;      // 메인 윈도우 해상도

    // FMOD 관리자 클래스
    FMOD::System* m_FMODSystem;     // FMOD 최상위 관리자 객체

public:
    const HWND GetMainWndHwnd() { return m_hMainWnd; }
    Vector2 GetResolution() { return m_vResolution; }

public:
    //_Save, _Load 함수 포인터 이름
    int Init(HINSTANCE _Inst, UINT _Width, UINT _Height, void(*_SAVE)(GGameObject*, FILE*), GGameObject* (_LOAD)(FILE*), GLevel* (_LOAD_LEVEL)(wstring));
    void Progress();
    FMOD::System* GetFMODSystem() { return m_FMODSystem; }
private:
    int CreateMainWindow();

};

