#pragma once
#include "GObjectBasic.h"
#include <Engine/GTransform.h>
#include <Engine/GPrefab.h>

#include "GGameManager.h"

class GFSM;

enum class PLAYER_STATE
{
	DEFAULT,
	WALK,
	JUMP,
	FALL,
	//SWIM,
	USEITEM,
	GETITEM,
	OPENBOX,
	FLINCH,
	ENDING,
	END
};

enum class PLAYER_FLIPBOOK
{
	DEFAULT,
	WALK,
	JUMP,
	FALL,
	HUG,
	HUG_READY,
	FLINCH,
	GETITEM,
	//OPENBOX,
	END
};

struct tKeyInput
{
	int HorizontalMove;
	bool Jump;			// 점프 키 Down
	bool JumpHold;		// 점프 키 홀드
	bool Interaction;
};

class GPlayer :
	public GObjectBasic
{
private:
	tKeyInput       m_KeyInput;
	GFSM*           m_FSM;

	PLAYER_STATE    m_PlayerState;							// 현재 상태

	float           m_MoveInitForce;
	float           m_MoveMaxSpeed;

	float			m_JumpTimeLimit;
	float			m_JumpTimeMin;
	float			m_JumpTimer;
	float			m_JumpPower;
	float			m_JumpMaxSpeed;



	bool            m_PlayerItems[(UINT)PLAYER_ITEM::END];	// 현재 가지고 있는 아이템
	PLAYER_ITEM		m_PlayerUseItem;						// 현재 사용중인 아이템 END라면 사용 중 X
	int				m_ItemMaxCount;

	float			m_ItemTimer;

	float			m_HookInitForce;
	float			m_HookMaxSpeed;

	GGameObject*	m_Bomb;
	Ptr<GPrefab>	m_BombPrefab;
	Vector2			m_BombDetectScale;
	float			m_BombTime;

	GGameObject*	m_Shovel;
	Ptr<GPrefab>	m_ShovelPrefab;
	GPlatform*		m_DestroyPlatform;
	float			m_ShovelTime;

	Vector2			m_HugDetectScale;
	float			m_HugTime;

	GGameObject*	m_Sword;
	Ptr<GPrefab>	m_SwordPrefab;
	Vector3			m_SwordPos;
	float			m_SwordTime;

public:
	GFSM* GetFSM() { return m_FSM; }

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Update() override;

	virtual void OnOverlapEnter(GCollider2D* _Other);
	virtual void OnOverlapStay(GCollider2D* _Other);
	virtual void OnOverlapExit(GCollider2D* _Other);

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);



private:
	void KeyInput();
	void UseItem();

	// 상호작용 체크
	bool Interaction();

	bool BoxCheck();

	bool ItemCheck();
	bool Hook();
	bool Bomb();
	bool Shovel();
	bool Hug();
	bool Sword();
	//bool ShotGun();
	//bool Shield();
	//bool Chicken();

public:
	CLONE(GPlayer);
public:
	GPlayer();
	GPlayer(const GPlayer& _Origin);
	~GPlayer();

	friend class GPlayerDefaultState;
	friend class GPlayerWalkState;
	friend class GPlayerUseItemState;
	friend class GPlayerJumpState;
	friend class GPlayerFallState;
	friend class GPlayerFlinchState;
	friend class GPlayerGetItemState;
};

