#pragma once
#include "GSingleton.h"

class GTimeManager : public GSingleton<GTimeManager>
{
private:
	SINGLE(GTimeManager);
private:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_PrevCount;
	LARGE_INTEGER m_CurCount;

	float m_TimeStep;

	UINT m_FPS;

	float m_TimeScale;

	float m_FDT;	// Fixed Delta Time : 물리 엔진용 DT, 시간이 튀어서 벽을 뚫는 현상을 억제한다.

	float m_DT;	// Delta TIme : 프레임 간격 시간, 1 프레임 동작하는데 걸리는 시간
	float m_Time;

	float m_EngineDT;	// Delta TIme : 프레임 간격 시간, 1 프레임 동작하는데 걸리는 시간
	float m_EngineTime;

	wchar_t m_Info[255];

public:
	void Init();
	void Progress();

	float GetTimeScale() { return m_TimeScale; }
	void SetTimeScale(float _TimeScale) { m_TimeScale = _TimeScale; }

	float FixedDeltaTime() { return m_FDT; }

	float DeltaTime() { return m_DT; }
	float AccTime() { return m_Time; }

	float EngineDeltaTime() { return m_EngineDT; }
	float EngineAccTime() { return m_EngineTime; }

	UINT FramePerSecond() { return m_FPS; }

	void PrintInfo();

	
};

