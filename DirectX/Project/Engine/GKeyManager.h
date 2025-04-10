#pragma once
#include "GSingleton.h"

// 키의 현재 상태를 나타내는 열거형
enum class KEY_STATE
{
	NONE,
	DOWN,
	PRESSED,
	UP,
};

// 사용할 수 있는 키를 나타내는 열거형
enum class KEY
{
	Q, W, E, R, T, Y,
	A, S, D, F, G, H,
	Z, X, C, V, B, N, M,

	ENTER,
	ESC,
	LSHIFT,
	ALT,
	CTRL,
	SPACE,

	UP, DOWN, LEFT, RIGHT,

	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,

	MINUS,
	PLUS,
	SLASH,

	LBTN,
	RBTN,

	KEY_END,
};

// 키의 정보 현재 상태, 이전에 눌렸는지를 저장하는 구조체
struct KeyInfo
{
	KEY_STATE	State;		// 현재 상태
	bool bPrevPressed;		// 이전 프레임에 눌렸는지
};

class GKeyManager :public GSingleton<GKeyManager>
{
	SINGLE(GKeyManager);
private:
	vector<KeyInfo>		m_vecKeyInfo;			// 키의 정보를 저장하는 배열

	Vector2				m_MousePos;
	Vector2				m_MousePrePos;
	Vector2				m_MouseDir;

public:
	KEY_STATE GetKeyState(KEY _key) { return m_vecKeyInfo[(UINT) _key].State; }		// 키의 현재 상태를 반환하는 함수
	Vector2 GetMousePos() { return m_MousePos; }
	Vector2 GetMouseDir() { return m_MouseDir; }
	Vector2 GetGameMousePos();

	bool IsMouseOffScreen();
public:
	void Init();
	void Progress();
};

