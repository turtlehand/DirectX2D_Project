#pragma once
#include <Engine/GSingleton.h>
#include <Engine/GSprite.h>

class GEndingCamera;
class GEndingScnene;

enum class ENDING_TYPE
{
	Older_Man_Attack,		// ���� ����
	Mighty_Minion,			// �̴Ͼ𿡰� ���
	Cruel_Minion_Killer,	// �̴Ͼ� ������ ���
	Mundane_Pouch,			// ������ ���� �� �ٸ� �η�����
	Disappointing_Hero,		// ���θ��� �Ѿ���
	Delicious_Strawberry,	// �� ����� ����
	Battleground,			// ���й�Ʋ�׶���
	Yahoo,					// ������ ���
	Lava,					// ���
	Taking_Day_Off,			// ������
	Stress_Relief,			// ���ֿ��� ��Ǯ��
	Execution,				// ���տ��� ó��
	Cowardly_Hero,			// ���� ��ġ
	Bear_Hug,				// �㸮�� �η��� ����
	Minon_Allergy,			// �̴Ͼ� �˷�����
	Rejection,				// ��¥
	LovePower,				// ����� ��
	Leap_of_Faith,			// �ŷ��� ����
	Abyss,
	Home,					// ���ֿ� ��
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
	void GameLoad();		// ���� �ʱ�ȭ
	void GameEnding(ENDING_TYPE _Type);

	friend class GGameManagerScript;
};

