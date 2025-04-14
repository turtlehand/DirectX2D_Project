#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMERAMOVE,
	FSM,
	GROUNDCHECKER,
	MONSTER,
	OBJECTBASIC,
	PLATFORM,
	PLAYER,
	PLAYERDEFAULTSTATE,
	PLAYERFALLSTATE,
	PLAYERJUMPSTATE,
	PLAYERUSEITEMSTATE,
	PLAYERWALKSTATE,
	WALLCHECKER,
};

using namespace std;

class GScript;

class GScriptManager
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static GScript * GetScript(const wstring& _strScriptName);
	static GScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(GScript * _pScript);
};
