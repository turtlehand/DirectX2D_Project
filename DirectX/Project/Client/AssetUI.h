#pragma once
#include "EditorUI.h"

#include <Engine/GAsset.h>
class AssetUI :
    public EditorUI
{
private:
    const ASSET_TYPE m_AssetType;
    Ptr<GAsset> m_Asset;

protected:
    void OutputTitle(const string& _Name);
    void OutputKeyPath();

public:
    void SetAsset(Ptr<GAsset> _Asset);
    Ptr<GAsset> GetAsset() { return m_Asset; }

public:
    AssetUI(ASSET_TYPE _Type);
    ~AssetUI();
};

