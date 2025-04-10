#include "pch.h"
#include "GScriptManager.h"

#include "CameraMove.h"
#include "Missile.h"
#include "Monster.h"
#include "Player.h"

void GScriptManager::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CameraMove");
	_vec.push_back(L"Missile");
	_vec.push_back(L"Monster");
	_vec.push_back(L"Player");
}

GScript * GScriptManager::GetScript(const wstring& _strScriptName)
{
	if (L"CameraMove" == _strScriptName)
		return new CameraMove;
	if (L"Missile" == _strScriptName)
		return new Missile;
	if (L"Monster" == _strScriptName)
		return new Monster;
	if (L"Player" == _strScriptName)
		return new Player;
	return nullptr;
}

GScript * GScriptManager::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVE:
		return new CameraMove;
		break;
	case (UINT)SCRIPT_TYPE::MISSILE:
		return new Missile;
		break;
	case (UINT)SCRIPT_TYPE::MONSTER:
		return new Monster;
		break;
	case (UINT)SCRIPT_TYPE::PLAYER:
		return new Player;
		break;
	}
	return nullptr;
}

const wchar_t * GScriptManager::GetScriptName(GScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVE:
		return L"CameraMove";
		break;

	case SCRIPT_TYPE::MISSILE:
		return L"Missile";
		break;

	case SCRIPT_TYPE::MONSTER:
		return L"Monster";
		break;

	case SCRIPT_TYPE::PLAYER:
		return L"Player";
		break;

	}
	return nullptr;
}