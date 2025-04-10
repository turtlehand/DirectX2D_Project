#pragma once
#include "EditorUI.h"

#include <Engine/GAsset.h>

class GGameObject;
class ComponentUI;
class AssetUI;
class ScriptUI;
class AddComponentUI;

class InspectorUI :
    public EditorUI
{
private:
    GGameObject*        m_Object;
    ComponentUI*        m_ComUI[(UINT)COMPONENT_TYPE::END];
    vector<ScriptUI*>   m_vecScriptUI;
    AddComponentUI*     m_AddComponentUI;

    Ptr<GAsset>         m_Asset;
    AssetUI*            m_AssetUI[(UINT)ASSET_TYPE::END];
public:
    void SetTargetObject(GGameObject* _Objcet);
    GGameObject* GetTargetObject() { return m_Object; }
    void SetAsset(Ptr<GAsset> _Asset);

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();

public:
    InspectorUI();
    ~InspectorUI();
};

