#include "pch.h"
#include "GFSM.h"

#include "GState.h"

GFSM::GFSM() :
	GScript(FSM),
	m_mapState{},
	m_CurState(nullptr)
{
	SetName(L"FSM");
}

GFSM::GFSM(const GFSM& _Origin)
	: GScript(FSM)
	, m_mapState()
	, m_CurState(nullptr)
{

}

GFSM::~GFSM()
{
	Delete_Map(m_mapState);
}

const wstring& GFSM::GetCurrentState()
{
	return m_CurState->GetName();
}

void GFSM::AddState(const wstring& _Key, GState* _State)
{
	assert(FindState(_Key) == nullptr);

	m_mapState.insert(make_pair(_Key, _State));
	_State->m_Owner = this;
	_State->SetName(_Key);
	_State->Awake();
}

GState* GFSM::FindState(const wstring& _Key)
{
	map<wstring, GState*>::iterator iter = m_mapState.find(_Key);

	if (iter == m_mapState.end())
		return nullptr;
	return iter->second;
}

void GFSM::ChanageState(const wstring& _Key, DWORD_PTR _Param1, DWORD_PTR _Param2)
{
	GState* pNextState = FindState(_Key);
	assert(pNextState != nullptr);

	if(m_CurState != nullptr)
		m_CurState->Exit();

	
	m_CurState = pNextState;
	m_CurStateName = ToString(_Key);
	if (_Param1 != 0 && _Param2 != 0)
		m_CurState->Enter(_Param1, _Param2);
	else if (_Param1 != 0 && _Param2 == 0)
		m_CurState->Enter(_Param1);
	else
		m_CurState->Enter();
}

void GFSM::Init()
{
	ADD_STRING("Cur State", &m_CurStateName);
}

void GFSM::Update()
{
	if (m_CurState == nullptr)
		return;
	m_CurState->Tick();
}

void GFSM::OnTriggerEnter(GCollider2D* _Other)
{
	if (m_CurState == nullptr)
		return;
	m_CurState->OnTriggerEnter(_Other);
}

void GFSM::OnTriggerStay(GCollider2D* _Other)
{
	if (m_CurState == nullptr)
		return;
	m_CurState->OnTriggerStay(_Other);
}

void GFSM::OnTriggerExit(GCollider2D* _Other)
{
	if (m_CurState == nullptr)
		return;
	m_CurState->OnTriggerExit(_Other);
}

void GFSM::SaveToFile(FILE* _File)
{
	return;
	// State ���� ����
	map<wstring, GState*>::iterator iter = m_mapState.begin();
	size_t Size = m_mapState.size();
	fwrite(&Size, sizeof(size_t), 1, _File);

	for (; iter != m_mapState.end(); ++iter)
	{
		GScript* pScript = dynamic_cast<GScript*>(iter->second);
		assert(pScript);

		// State Ÿ�� ����
		wstring ScriptName = GScriptManager::GetScriptName(pScript);
		SaveWString(ScriptName, _File);

		// State Key ����
		wstring ScriptKey = iter->first;
		SaveWString(ScriptKey, _File);

		// �ش� State ���� ����
		pScript->SaveToFile(_File);
	}
}

void GFSM::LoadFromFile(FILE* _File)
{
	return;
	// ��ũ��Ʈ ���� �ҷ�����
	size_t Size;
	fread(&Size, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < Size; ++i)
	{
		GScript* pScript = nullptr;

		// ��ũ��Ʈ Ÿ�� �ҷ�����
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// State Key ����
		wstring ScriptKey = L"";
		LoadWString(ScriptKey, _File);

		// ��ũ��Ʈ Ÿ������ ��ũ��Ʈ�� �������Ѽ� ������Ʈ�� �־���
		pScript = GScriptManager::GetScript(ScriptName);
		GState* pState = dynamic_cast<GState*>(pScript);
		assert(pState);

		AddState(ScriptKey, pState);

		// ��ũ��Ʈ�� ������ �����͸� �ٽ� ������
		pScript->LoadFromFile(_File);
	}
}

