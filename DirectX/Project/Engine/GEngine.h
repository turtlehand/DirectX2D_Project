#pragma once
#include "GSingleton.h"

class GLevel;

class GEngine :
    public GSingleton<GEngine>
{
    SINGLE(GEngine);
private:
    HINSTANCE   m_hInst;          // ���μ��� �ּ�
    HWND        m_hMainWnd;            // ���� ������ �ڵ�
    Vector2     m_vResolution;      // ���� ������ �ػ�

    // FMOD ������ Ŭ����
    FMOD::System* m_FMODSystem;     // FMOD �ֻ��� ������ ��ü

public:
    const HWND GetMainWndHwnd() { return m_hMainWnd; }
    Vector2 GetResolution() { return m_vResolution; }

public:
    //_Save, _Load �Լ� ������ �̸�
    int Init(HINSTANCE _Inst, UINT _Width, UINT _Height, void(*_SAVE)(GGameObject*, FILE*), GGameObject* (_LOAD)(FILE*), GLevel* (_LOAD_LEVEL)(wstring));
    void Progress();
    FMOD::System* GetFMODSystem() { return m_FMODSystem; }
private:
    int CreateMainWindow();

};

