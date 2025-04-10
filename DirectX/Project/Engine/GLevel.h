#pragma once
#include "GEntity.h"


class GLayer;
class GGameObject;

class GLevel :
    public GEntity
{
private:
    GLayer* m_Layer[MAX_LAYER];
    LEVEL_STATE m_State;

public:
    void AddGameObject(GGameObject* _Object, int _LayerIdx, bool _bChildMove = false);
    LEVEL_STATE GetState() { return m_State; }
    void ChangeState(LEVEL_STATE _NextState);
    GGameObject* FindObjectByName(const wstring& _Name);
public:
    GLayer* GetLayer(int _Layer) { return m_Layer[_Layer]; }
    void Deregister();


public:
    void Begin();
    void Update();
    void FinalUpdate();

public:
    CLONE(GLevel)

public:
    GLevel();
    ~GLevel();

private:
    GLevel(const GLevel& _Origin);
};

