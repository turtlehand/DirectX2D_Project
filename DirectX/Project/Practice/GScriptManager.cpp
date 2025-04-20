#include "pch.h"
#include "GScriptManager.h"

#include "GCameraMove.h"
#include "GCeilingChecker.h"
#include "GEndingCamera.h"
#include "GEndingScene.h"
#include "GFSM.h"
#include "GGroundChecker.h"
#include "GItem.h"
#include "GLord.h"
#include "GMinion.h"
#include "GPlatform.h"
#include "GPlayer.h"
#include "GPlayerDefaultState.h"
#include "GPlayerFallState.h"
#include "GPlayerJumpState.h"
#include "GPlayerWalkState.h"
#include "GPrincess.h"

void GScriptManager::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"GCameraMove");
	_vec.push_back(L"GCeilingChecker");
	_vec.push_back(L"GEndingCamera");
	_vec.push_back(L"GEndingScene");
	_vec.push_back(L"GFSM");
	_vec.push_back(L"GGroundChecker");
	_vec.push_back(L"GItem");
	_vec.push_back(L"GLord");
	_vec.push_back(L"GMinion");
	_vec.push_back(L"GPlatform");
	_vec.push_back(L"GPlayer");
	_vec.push_back(L"GPlayerDefaultState");
	_vec.push_back(L"GPlayerFallState");
	_vec.push_back(L"GPlayerJumpState");
	_vec.push_back(L"GPlayerWalkState");
	_vec.push_back(L"GPrincess");
}

GScript * GScriptManager::GetScript(const wstring& _strScriptName)
{
	if (L"GCameraMove" == _strScriptName)
		return new GCameraMove;
	if (L"GCeilingChecker" == _strScriptName)
		return new GCeilingChecker;
	if (L"GEndingCamera" == _strScriptName)
		return new GEndingCamera;
	if (L"GEndingScene" == _strScriptName)
		return new GEndingScene;
	if (L"GFSM" == _strScriptName)
		return new GFSM;
	if (L"GGroundChecker" == _strScriptName)
		return new GGroundChecker;
	if (L"GItem" == _strScriptName)
		return new GItem;
	if (L"GLord" == _strScriptName)
		return new GLord;
	if (L"GMinion" == _strScriptName)
		return new GMinion;
	if (L"GPlatform" == _strScriptName)
		return new GPlatform;
	if (L"GPlayer" == _strScriptName)
		return new GPlayer;
	if (L"GPlayerDefaultState" == _strScriptName)
		return new GPlayerDefaultState;
	if (L"GPlayerFallState" == _strScriptName)
		return new GPlayerFallState;
	if (L"GPlayerJumpState" == _strScriptName)
		return new GPlayerJumpState;
	if (L"GPlayerWalkState" == _strScriptName)
		return new GPlayerWalkState;
	if (L"GPrincess" == _strScriptName)
		return new GPrincess;
	return nullptr;
}

GScript * GScriptManager::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVE:
		return new GCameraMove;
		break;
	case (UINT)SCRIPT_TYPE::CEILINGCHECKER:
		return new GCeilingChecker;
		break;
	case (UINT)SCRIPT_TYPE::ENDINGCAMERA:
		return new GEndingCamera;
		break;
	case (UINT)SCRIPT_TYPE::ENDINGSCENE:
		return new GEndingScene;
		break;
	case (UINT)SCRIPT_TYPE::FSM:
		return new GFSM;
		break;
	case (UINT)SCRIPT_TYPE::GROUNDCHECKER:
		return new GGroundChecker;
		break;
	case (UINT)SCRIPT_TYPE::ITEM:
		return new GItem;
		break;
	case (UINT)SCRIPT_TYPE::LORD:
		return new GLord;
		break;
	case (UINT)SCRIPT_TYPE::MINION:
		return new GMinion;
		break;
	case (UINT)SCRIPT_TYPE::PLATFORM:
		return new GPlatform;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER:
		return new GPlayer;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERDEFAULTSTATE:
		return new GPlayerDefaultState;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERFALLSTATE:
		return new GPlayerFallState;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERJUMPSTATE:
		return new GPlayerJumpState;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERWALKSTATE:
		return new GPlayerWalkState;
		break;
	case (UINT)SCRIPT_TYPE::PRINCESS:
		return new GPrincess;
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

	case SCRIPT_TYPE::CEILINGCHECKER:
		return L"GCeilingChecker";
		break;

	case SCRIPT_TYPE::ENDINGCAMERA:
		return L"GEndingCamera";
		break;

	case SCRIPT_TYPE::ENDINGSCENE:
		return L"GEndingScene";
		break;

	case SCRIPT_TYPE::FSM:
		return L"GFSM";
		break;

	case SCRIPT_TYPE::GROUNDCHECKER:
		return L"GGroundChecker";
		break;

	case SCRIPT_TYPE::ITEM:
		return L"GItem";
		break;

	case SCRIPT_TYPE::LORD:
		return L"GLord";
		break;

	case SCRIPT_TYPE::MINION:
		return L"GMinion";
		break;

	case SCRIPT_TYPE::PLATFORM:
		return L"GPlatform";
		break;

	case SCRIPT_TYPE::PLAYER:
		return L"GPlayer";
		break;

	case SCRIPT_TYPE::PLAYERDEFAULTSTATE:
		return L"GPlayerDefaultState";
		break;

	case SCRIPT_TYPE::PLAYERFALLSTATE:
		return L"GPlayerFallState";
		break;

	case SCRIPT_TYPE::PLAYERJUMPSTATE:
		return L"GPlayerJumpState";
		break;

	case SCRIPT_TYPE::PLAYERWALKSTATE:
		return L"GPlayerWalkState";
		break;

	case SCRIPT_TYPE::PRINCESS:
		return L"GPrincess";
		break;

	}
	return nullptr;
}