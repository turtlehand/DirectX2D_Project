#pragma once
#include <Engine/GSingleton.h>
#include <Engine/GTexture.h>

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

	END
};

class GPlayer;

class GGameManager
	: public GSingleton<GGameManager>
{
private:
	SINGLE(GGameManager);
	bool m_Ending[(int)ENDING_TYPE::END];
	Ptr<GTexture> m_EndingScene[(int)ENDING_TYPE::END];

private:
	GPlayer* m_Player;
	bool m_IsEnd;
	float m_EndingTime;
	float m_EndingTimer;

	class GEndingCamera* m_Camera;
	class GEndingScene* m_Scene;

public:
	void Init();
	void Progress();

public:
	void SaveGameEnding();
	void GameReset();		// ���� �ʱ�ȭ
	void GameEnding(ENDING_TYPE _Type);
};

