#pragma once
#include "GSingleton.h"

// Ű�� ���� ���¸� ��Ÿ���� ������
enum class KEY_STATE
{
	NONE,
	DOWN,
	PRESSED,
	UP,
};

// ����� �� �ִ� Ű�� ��Ÿ���� ������
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

// Ű�� ���� ���� ����, ������ ���ȴ����� �����ϴ� ����ü
struct KeyInfo
{
	KEY_STATE	State;		// ���� ����
	bool bPrevPressed;		// ���� �����ӿ� ���ȴ���
};

class GKeyManager :public GSingleton<GKeyManager>
{
	SINGLE(GKeyManager);
private:
	vector<KeyInfo>		m_vecKeyInfo;			// Ű�� ������ �����ϴ� �迭

	Vector2				m_MousePos;
	Vector2				m_MousePrePos;
	Vector2				m_MouseDir;

public:
	KEY_STATE GetKeyState(KEY _key) { return m_vecKeyInfo[(UINT) _key].State; }		// Ű�� ���� ���¸� ��ȯ�ϴ� �Լ�
	Vector2 GetMousePos() { return m_MousePos; }
	Vector2 GetMouseDir() { return m_MouseDir; }
	Vector2 GetGameMousePos();

	bool IsMouseOffScreen();
public:
	void Init();
	void Progress();
};

