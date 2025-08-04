#include "pch.h"
#include "GEngine.h"

#include "GDevice.h"
#include "GPathManager.h"
#include "GKeyManager.h"
#include "GTimeManager.h"
#include "GAssetManager.h"
#include "GLevelManager.h"
#include "GRenderManager.h"
#include "GCollisionManager.h"
#include "GTaskManager.h"

#include "GPrefab.h"
#include "GLevel.h"

GEngine::GEngine()
    : m_hInst(nullptr)
    , m_hMainWnd(nullptr)
    , m_FMODSystem(nullptr)
{

}

GEngine::~GEngine()
{
    if (nullptr != m_FMODSystem)
    {
        m_FMODSystem->release();
        m_FMODSystem = nullptr;
    }
}

int GEngine::Init(HINSTANCE _Inst, UINT _Width, UINT _Height
    , void(*_SAVE)(GGameObject*, FILE*), GGameObject* (_LOAD)(FILE*), GLevel* (_LOAD_LEVEL)(wstring))
{
    m_hInst = _Inst;
    m_vResolution = Vector2(_Width, _Height);

    // 프리팹 세이브 로드 함수 주소 등록
    GPrefab::g_Save = (GAMEOBJECT_SAVE)_SAVE;
    GPrefab::g_Load = (GAMEOBJECT_LOAD)_LOAD;
    GLevelManager::g_Load_Level = (LEVEL_LOAD)_LOAD_LEVEL;

    if (FAILED(CreateMainWindow()))
    {
        return E_FAIL;
    }

    // Device 초기화
    if (FAILED(GDevice::GetInst()->Init(m_hMainWnd, m_vResolution)))
    {
        return E_FAIL;
    }

    // FMOD 초기화		
    FMOD::System_Create(&m_FMODSystem);
    assert(m_FMODSystem);

    // 32개 채널 생성
    m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);

    // Manager 초기화
    GPathManager::Init();
    GKeyManager::GetInst()->Init();
    GTimeManager::GetInst()->Init();
    GAssetManager::GetInst()->Init();
    GLevelManager::GetInst()->Init();
    GRenderManager::GetInst()->Init();

    return S_OK;
}

void GEngine::Progress()
{
    // Manager Tick
    GTimeManager::GetInst()->Progress();
    GKeyManager::GetInst()->Progress();

    // FMOD Tick
    m_FMODSystem->update();

    // Level Progress
    GLevelManager::GetInst()->Progress();

    // Collision Check
    GCollisionManager::GetInst()->Progress();

    // Render
    GRenderManager::GetInst()->Progress();

    // Task
    GTaskManager::GetInst()->Progress();
}

int GEngine::CreateMainWindow()
{
    m_hMainWnd = CreateWindowW(L"MyWindow", L"DirectX53", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hMainWnd)
        return E_FAIL;

    ShowWindow(m_hMainWnd, true);
    UpdateWindow(m_hMainWnd);

    // 입력된 해상도에 맞는 윈도우 크기 설정
    RECT rt = { 0, 0, (int)m_vResolution.x, (int)m_vResolution.y };
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, !!GetMenu(m_hMainWnd));
    SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

    return S_OK;
}
