#pragma once
#include "ComponentUI.h"
class ScriptUI :
    public ComponentUI
{
private:
    class GScript* m_TargetScript;

public:
    void SetScript(GScript* _Script);
    GScript* GetScript() { return m_TargetScript; }

protected:
    virtual void DeleteComponent() override;

public:
    virtual void Render_UI() override;

public:
    ScriptUI();
    ~ScriptUI();
};

