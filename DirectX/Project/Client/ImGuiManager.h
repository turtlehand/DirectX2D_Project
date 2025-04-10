#pragma once

class EditorUI;

typedef void(EditorUI::* EUI_DELEGATE_0)(void);
typedef void(EditorUI::* EUI_DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* EUI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

class ImGuiManager :
    public GSingleton<ImGuiManager>
{
    SINGLE(ImGuiManager);
private:
    map<string, EditorUI*> m_mapUI;

public:
    template<typename T>
    T* FindUI(string _Name)
    {
        map<string, EditorUI*>::iterator iter = m_mapUI.find(_Name);

        if (iter == m_mapUI.end())
            return nullptr;

        return dynamic_cast<T*>(iter->second);
    }

public:
    void Init(HWND _hMainWind, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _Context);
    void Progress();

public:
    void CreateGameObject(GGameObject* _Parent = nullptr);
    void DeleteGameObject(GGameObject* _Object);

private:
    void CreateEditorUI();
};

