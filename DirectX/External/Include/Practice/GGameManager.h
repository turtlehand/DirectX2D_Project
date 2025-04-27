#pragma once
#include <Engine/GSingleton.h>
#include <Engine/GSprite.h>

class GEndingCamera;
class GEndingScnene;

enum class ENDING_TYPE
{
	Older_Man_Attack,		// 노인 공격
	Mighty_Minion,			// 미니언에게 사망
	Cruel_Minion_Killer,	// 미니언 여러번 찌르기
	Mundane_Pouch,			// 아이템 많이 들어서 다리 부러지기
	Disappointing_Hero,		// 돌부리에 넘어짐
	Delicious_Strawberry,	// 산 꼭대기 딸기
	Battleground,			// 갑분배틀그라운드
	Yahoo,					// 마리오 토관
	Lava,					// 용암
	Taking_Day_Off,			// 집돌이
	Stress_Relief,			// 공주에게 분풀이
	Execution,				// 마왕에게 처형
	Cowardly_Hero,			// 마왕 뒤치
	Bear_Hug,				// 허리가 부러진 노인
	Minon_Allergy,			// 미니언 알레르기
	Rejection,				// 퇴짜
	LovePower,				// 사랑의 힘
	Leap_of_Faith,			// 신뢰의 도약
	Abyss,
	Home,					// 공주와 집
	Hurt_From_a_Fall,
	Pressed,

	END
};

enum class PLAY_TYPE
{
	PLAY,
	PAUSE,
	ENDING_SCENE,
	RETRY,
	END,
};

extern const vector<string> EndingName;

class GPlayer;
class GPrefab;

class GGameManager
	: public GSingleton<GGameManager>
{

	SINGLE(GGameManager);

private:
	bool m_Ending[(int)ENDING_TYPE::END];
	Ptr<GSprite> m_EndingScene[(int)ENDING_TYPE::END];

	Ptr<GPrefab>	m_DarkLord;
private:
	GPlayer* m_Player;
	PLAY_TYPE m_PlayType;
	float m_EndingTime;
	float m_EndingTimer;

	class GEndingCamera* m_Camera;
	class GEndingScene* m_Scene;

public:
	PLAY_TYPE GetPlayType() { return m_PlayType; }
	bool IsEnd() { return PLAY_TYPE::ENDING_SCENE == m_PlayType; }
	bool IsPause() {return PLAY_TYPE::PAUSE == m_PlayType; }

	void CallDarkLord();

public:
	void Init();
	void Begin();
	void Progress();
	void End();

public:
	void SaveGameEnding();
	void GameLoad();		// 상태 초기화
	void GameEnding(ENDING_TYPE _Type);

	friend class GGameManagerScript;
};

