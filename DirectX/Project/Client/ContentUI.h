#pragma once
#include "EditorUI.h"
class ContentUI :
    public EditorUI
{
private:
    class TreeUI*       m_Tree;
    /// <summary>
    /// 파일 이름
    /// 상대경로 포함하여 
    /// </summary>
    vector<wstring>     m_ContentFile; 
    /// <summary>
    /// Content 폴더에 변경점이 생겼는지 확인하는 커널객체 핸들
    /// </summary>
    HANDLE              m_NotifyHandle;

public:
    void Selection(DWORD_PTR _Node);
    void Renew();
    void AddPrefab(DWORD_PTR _PayLoad, DWORD_PTR _TargetAsset); //Object ,Prefab

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;


public:
    void FindContentFile(const wstring& _FolderPath);
    void Reload();
    void ObserveContent();

public:
    ContentUI();
    ~ContentUI();
};

