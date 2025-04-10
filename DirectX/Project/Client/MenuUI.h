#pragma once
#include "EditorUI.h"
#include <Engine/GPathManager.h>
#include <Engine/GAssetManager.h>
#include <Engine/GAsset.h>

class GLevel;

class MenuUI :
    public EditorUI
{
private:
    bool m_FlipbookEditor;

    GLevel* m_CloneLevel;

private:
    void File();
    void Setting();
    void Level();
    void Editor();
    void Script();
    void Asset();

    template<typename T>
    wstring GetDefualtAssetName(wstring _AssetName);   // 중복 되지 않는 이름으로 Default Material (index)를 찾는다.
    template<typename T>
    Ptr<T> CreateDefaultAsset(wstring _AssetName);
public:
    virtual void Render() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    MenuUI();
    ~MenuUI();
};

template<typename T>
inline wstring MenuUI::GetDefualtAssetName(wstring _AssetName)
{
    wstring AssetName = _AssetName + L"\\" + L"Default " + _AssetName + L" ";
    ASSET_TYPE Type = GetAssetType<T>();
    assert(Type != ASSET_TYPE::END);
    wstring Ext = GAssetManager::GetInst()->GetTypeToExt(Type);

    int i = 0;
    while (true)
    {
        wstring Name = AssetName + to_wstring(i++) + Ext;
        Ptr<T> pAsset = GAssetManager::GetInst()->FindAsset<T>(Name);
        if (nullptr == pAsset)
        {
            AssetName = Name;
            break;
        }
    }

    
    return AssetName;
}

template<typename T>
inline Ptr<T> MenuUI::CreateDefaultAsset(wstring _AssetName)
{
    ASSET_TYPE Type = GetAssetType<T>();
    assert(Type != ASSET_TYPE::END);

    // AssetManager에 Asset 추가
    Ptr<T> pAsset = new T;
    wstring AssetName = GetDefualtAssetName<T>(_AssetName);

    // 상대 경로를 이름으로 저장
    GAssetManager::GetInst()->AddAsset(AssetName, pAsset.Get());

    // 컨텐츠 경로에 저장 파일 추가
    wstring Path = GPathManager::GetContentPath() + AssetName;
    pAsset->Save(Path);
    return pAsset;
}
