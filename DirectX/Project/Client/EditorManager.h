#pragma once

class GGameObjectEX;

class EditorManager :
    public GSingleton<EditorManager>
{
    SINGLE(EditorManager)
private:
    vector<GGameObjectEX*>  m_vecEditorObject;
    GGameObjectEX* m_EditorCamera;

public:
    const GGameObjectEX* GetEditorCamera() { return m_EditorCamera; }

public:
    void Init();
    void Progress();

};

