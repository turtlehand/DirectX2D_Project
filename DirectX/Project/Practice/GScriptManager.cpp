#include "pch.h"
#include "GScriptManager.h"

#include "GCameraMove.h"
#include "GFSM.h"
#include "GMonster.h"
#include "GPlayer.h"
#include "GPlayerDefaultState.h"
#include "GPlayerWalkState.h"

void GScriptManager::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"GCameraMove");
	_vec.push_back(L"GFSM");
	_vec.push_back(L"GMonster");
	_vec.push_back(L"GPlayer");
	_vec.push_back(L"GPlayerDefaultState");
	_vec.push_back(L"GPlayerWalkState");
}

GScript * GScriptManager::GetScript(const wstring& _strScriptName)
{
	if (L"GCameraMove" == _strScriptName)
		return new GCameraMove;
	if (L"GFSM" == _strScriptName)
		return new GFSM;
	if (L"GMonster" == _strScriptName)
		return new GMonster;
	if (L"GPlayer" == _strScriptName)
		return new GPlayer;
	if (L"GPlayerDefaultState" == _strScriptName)
		return new GPlayerDefaultState;
	if (L"GPlayerWalkState" == _strScriptName)
		return new GPlayerWalkState;
	return nullptr;
}

GScript * GScriptManager::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVE:
		return new GCameraMove;
		break;
	case (UINT)SCRIPT_TYPE::FSM:
		return new GFSM;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER:
		return new GMonster;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER:
		return new GPlayer;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERDEFAULTSTATE:
		return new GPlayerDefaultState;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERWALKSTATE:
		return new GPlayerWalkState;
		break;
	}
	return nullptr;
}

const wchar_t * GScriptManager::GetScriptName(GScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVE:
		return L"GCameraMove";
		break;

	case SCRIPT_TYPE::FSM:
		return L"GFSM";
		break;

	case SCRIPT_TYPE::MONSTER:
		return L"GMonster";
		break;

	case SCRIPT_TYPE::PLAYER:
		return L"GPlayer";
		break;

	case SCRIPT_TYPE::PLAYERDEFAULTSTATE:
		return L"GPlayerDefaultState";
		break;

	case SCRIPT_TYPE::PLAYERWALKSTATE:
		return L"GPlayerWalkState";
		break;

	}
	return nullptr;
}