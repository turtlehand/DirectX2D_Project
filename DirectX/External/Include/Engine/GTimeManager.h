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

	float m_FDT;	// Fixed Delta Time : ���� ������ DT, �ð��� Ƣ� ���� �մ� ������ �����Ѵ�.

	float m_DT;	// Delta TIme : ������ ���� �ð�, 1 ������ �����ϴµ� �ɸ��� �ð�
	float m_Time;

	float m_EngineDT;	// Delta TIme : ������ ���� �ð�, 1 ������ �����ϴµ� �ɸ��� �ð�
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

