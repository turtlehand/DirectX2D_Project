#pragma once
#include "GComponent.h"

#include "GPrefab.h"

enum class SCRIPT_PARAM
{
    BOOL,
    INT,
    ENUM,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    STRING,
    TEXTURE,
    SPRITE,
    SOUND,
    PREFAB,
};

struct tScriptParam
{
    SCRIPT_PARAM    Type;
    string          Desc;
    void*           pData;
    DWORD_PTR       Param0;
    DWORD_PTR       Param1;
    DWORD_PTR       Param2;
};

class GScript :
    public GComponent
{
private:
    const int m_ScriptType;
    vector<tScriptParam> m_vecScriptParam;

public:
    int GetScriptType() const { return m_ScriptType; }
    void AddScriptParam(SCRIPT_PARAM _Type, const string& _Desc, void* _Data
        , DWORD_PTR _Param0 = 0, DWORD_PTR _Param1 = 0, DWORD_PTR _Param2 = 0)
    {
        m_vecScriptParam.push_back(tScriptParam{ _Type , _Desc , _Data , _Param0 , _Param1 , _Param2 });
    }

    const vector<tScriptParam>& GetScriptParam() { return m_vecScriptParam; }

public:
    virtual void FinalUpdate() final;
    virtual void Update() = 0;

    virtual void OnOverlapEnter(GCollider2D* _Other) {};
    virtual void OnOverlapStay(GCollider2D* _Other) {};
    virtual void OnOverlapExit(GCollider2D* _Other) {};

public:
    void Instantiate(Ptr<GPrefab> _Prefab, Vector3 _WorldPos = Vector3(0.f,0.f,0.f));

public:
    virtual GScript* Clone() = 0;

public:
    GScript(int _ScriptType);
    GScript(const GScript& _Origin);
    ~GScript();
};

