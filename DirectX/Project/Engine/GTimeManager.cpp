#include "pch.h"
#include "GTimeManager.h"

#include "GEngine.h"
#include "GLevelManager.h"
#include "GLevel.h"

GTimeManager::GTimeManager() :
	m_Frequency{},
	m_PrevCount{},
	m_CurCount{},
	m_FPS(0),
	m_DT(0.f),
	m_Time(0.f),
	m_EngineDT(0.f),	// Delta TIme : 프레임 간격 시간, 1 프레임 동작하는데 걸리는 시간
	m_EngineTime(0.f),
	m_Info{}
{
}

GTimeManager::~GTimeManager()
{

}

void GTimeManager::Init()
{
	// 1c초에 1000 카운팅 -> 초당 카운팅 양이 너무 적음
	// GetTIckCount();
	
	// 프레임과 프레임 사이의 시간
	QueryPerformanceFrequency(&m_Frequency);	// 초당 카운팅 값 (천만)
	QueryPerformanceCounter(&m_PrevCount);
}

void GTimeManager::Progress()
{
	// 현재 시점 Count 값
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDT = m_DT = (float)(m_CurCount.QuadPart - m_PrevCount.QuadPart) / (float)m_Frequency.QuadPart;
	m_PrevCount = m_CurCount;
	++m_FPS;

	// Level의 상태값 체크
	LEVEL_STATE State = GLevelManager::GetInst()->GetCurrentLevelState();

	if (LEVEL_STATE::PLAY != State) {
		m_DT = 0.f;
	}
	else
	{
		m_Time += m_DT;
	}

	m_EngineTime += m_EngineDT;



	// 전역 상수 데이터 갱싱
	g_Global.g_DT = m_DT;
	g_Global.g_EngineDT = m_EngineDT;

	g_Global.g_AccTime = m_Time;
	g_Global.g_EnginAccTime = m_EngineTime;

	static float OneSecond = 0.f;
	OneSecond += m_EngineDT;
	if (1.f <= OneSecond)
	{
		wchar_t buff[255] = {};
		swprintf_s(buff, 255, L"FPS : %d, DeltaTime : %f", m_FPS, m_DT);
		SetWindowText(GEngine::GetInst()->GetMainWndHwnd(), buff);

		m_FPS = 0;
		OneSecond -= 1.f;
	}



}

void GTimeManager::PrintInfo()
{
	//TextOut(GEngine::GetInst()->GetSecondDC(), 0, 0, m_Info, wcslen(m_Info));
}
