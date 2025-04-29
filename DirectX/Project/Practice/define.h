#pragma once

#define GAME_RELEASED


#ifdef GAME_RELEASED
#define ADD_BOOL(Desc, pData)
#define ADD_INT(Desc, pData)
#define ADD_ENUM(Desc, pData, EnumName)
#define ADD_FLOAT(Desc, pData)
#define ADD_SPRITE(Desc, pData)
#define ADD_SOUND(Desc, pData)
#define ADD_PREFAB(Desc, pData)
#define ADD_STRING(Desc, pData);
#define ADD_VECTOR2(Desc, pData)
#define ADD_VECTOR3(Desc, pData)

#else
#define ADD_BOOL(Desc, pData) AddScriptParam(SCRIPT_PARAM::BOOL, Desc, pData);
#define ADD_INT(Desc, pData) AddScriptParam(SCRIPT_PARAM::INT, Desc, pData);
#define ADD_ENUM(Desc, pData, EnumName) AddScriptParam(SCRIPT_PARAM::ENUM, Desc, pData, EnumName);
#define ADD_SPRITE(Desc, pData) AddScriptParam(SCRIPT_PARAM::SPRITE, Desc, pData);
#define ADD_FLOAT(Desc, pData) AddScriptParam(SCRIPT_PARAM::FLOAT, Desc, pData);
#define ADD_SOUND(Desc, pData) AddScriptParam(SCRIPT_PARAM::SOUND, Desc, pData);
#define ADD_PREFAB(Desc, pData) AddScriptParam(SCRIPT_PARAM::PREFAB, Desc, pData);
#define ADD_STRING(Desc, pData) AddScriptParam(SCRIPT_PARAM::STRING, Desc, pData);
#define ADD_VECTOR2(Desc, pData) AddScriptParam(SCRIPT_PARAM::VEC2, Desc, pData);
#define ADD_VECTOR3(Desc, pData) AddScriptParam(SCRIPT_PARAM::VEC3, Desc, pData);

#endif
