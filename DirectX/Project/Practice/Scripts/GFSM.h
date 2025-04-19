#pragma once
#include <Engine/GScript.h>

class GState;

class GFSM : public GScript
{
private:
	map<wstring, GState*> m_mapState;
	GState* m_CurState;
	string m_CurStateName;
public:
	const wstring& GetCurrentState();
	void AddState(const wstring& _Key, GState* _State);
	GState* FindState(const wstring& _Key);
	void ChanageState(const wstring& _Key, DWORD_PTR _Param1 =0, DWORD_PTR _Param2 = 0);

public:
	virtual void Init() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(GCollider2D* _Other);
	virtual void OnTriggerStay(GCollider2D* _Other);
	virtual void OnTriggerExit(GCollider2D* _Other);

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	CLONE(GFSM);

public:
	GFSM();
	GFSM(const GFSM& _Origin);
	virtual ~GFSM() override;
};

