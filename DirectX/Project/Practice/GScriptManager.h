#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	CAMERAMOVE,
	MISSILE,
	MONSTER,
	PLAYER,
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
