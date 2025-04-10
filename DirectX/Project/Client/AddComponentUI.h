#pragma once
#include "EditorUI.h"
class AddComponentUI :
    public EditorUI
{
    GGameObject* m_Target;

public:
    void SetTarget(GGameObject* _Target);

private:
    void SelectComponent(DWORD_PTR _Data);

public:
    virtual void Update_UI() override {};
    virtual void Render_UI() override;

public:
    AddComponentUI();
    ~AddComponentUI();
};

