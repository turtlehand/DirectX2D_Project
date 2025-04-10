#pragma once
#include "EditorUI.h"
#include "ImGuiManager.h"

class ListUI :
    public EditorUI
{
private:
    vector<string> m_vecStr;
    int m_Idx;
    string m_Selected_string;
    UINT m_Selected_int;

    EditorUI* m_TargetUI;
    //EUI_DELEGATE_0 m_MemFun0;
    EUI_DELEGATE_1 m_MemFun1;
    //EUI_DELEGATE_2 m_MemFun2;

public:
    void SetItems(const vector<string>& _Items);
    void AddDelegate(EditorUI* _TargetUI, EUI_DELEGATE_1 _MemFun1);

    const string& GetString() { return m_Selected_string; }
    int GetInt() { return m_Selected_int; }

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    ListUI();
    ~ListUI();
};

