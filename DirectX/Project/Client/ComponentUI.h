#pragma once
#include "EditorUI.h"
#include <Engine/GGameObject.h>

class ComponentUI :
    public EditorUI
{
private:
    const COMPONENT_TYPE m_Type; // 담당 컴포넌트 타입
    GGameObject* m_Target;
    float m_ItemHeight;

protected:
    void OutputTitle(const string& _Name);
    void AddItemHeight();
    virtual void DeleteComponent();

public:
    void SetTarget(GGameObject* _Target);
    GGameObject*& GetTarget() { return m_Target; }

    virtual void Update_UI() override;

public:
    ComponentUI(COMPONENT_TYPE _Type);
    ~ComponentUI();
};

