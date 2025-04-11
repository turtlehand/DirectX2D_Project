#pragma once
#include <Engine/GScript.h>
#include <Engine/GPrefab.h>

enum class PLAYER_STATE
{
	DEFAULT,
	WALK,
	JUMP,
	FALL,
	SWIM,
	USEITEM,
	GETITEM,
	OPENBOX,
	FLINCH,
	ENDING,
	END
};

struct tKeyInput
{
	int HorizontalMove;
	bool Jump;
	bool Interaction;
};

enum class PLAYER_ITEM
{
	HOOK,
	BOMB,
	SHOVEL,
	HUG,
	SHOT_GUN,
	SWORD,
	SHIELD,
	CHICKEN,
	END
};

class GFSM;

class GPlayer :
	public GScript
{
private:
	tKeyInput       m_KeyInput;
	GFSM*           m_FSM;

	float           m_MoveInitForce;
	float           m_MoveMaxSpeed;

	PLAYER_STATE    m_PlayerState;							// ���� ����
	bool            m_PlayerItems[(UINT)PLAYER_ITEM::END];	// ���� ������ �ִ� ������
	PLAYER_ITEM		m_PlayerUseItem;						// ���� ������� ������ END��� ��� �� X

	float			m_HookInitForce;
	float			m_HookMaxSpeed;

public:
	GFSM* GetFSM() { return m_FSM; }

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(GCollider2D* _Other);
	virtual void OnTriggerStay(GCollider2D* _Other);
	virtual void OnTriggerExit(GCollider2D* _Other);

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	// �ش� ������ �ٶ󺸰� �Ѵ�.
	void SetMoveDirection(int _Direction);

private:
	void KeyInput();

	bool Interaction();

	bool BoxCheck();

	bool ItemCheck();
	bool Hook();
	bool Bomb();
	bool Shovel();
	bool Hug();
	bool Sword();
	bool ShotGun();
	bool Shield();
	bool Chicken();



public:
	CLONE(GPlayer);
public:
	GPlayer();
	~GPlayer();

	friend class GPlayerDefaultState;
	friend class GPlayerWalkState;
	friend class GPlayerUseItemState;
};

