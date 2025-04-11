#pragma once

//#define GAME_RELEASED


#ifdef GAME_RELEASED
#define ADD_BOOL(Desc, pData)
#define ADD_INT(Desc, pData)
#define ADD_FLOAT(Desc, pData)

#else
#define ADD_BOOL(Desc, pData) AddScriptParam(SCRIPT_PARAM::BOOL, Desc, pData);
#define ADD_INT(Desc, pData) AddScriptParam(SCRIPT_PARAM::INT, Desc, pData);
#define ADD_FLOAT(Desc, pData) AddScriptParam(SCRIPT_PARAM::FLOAT, Desc, pData);

#endif
