#pragma once
#include "GAsset.h"
#include "GSprite.h"

class GFlipbook :
    public GAsset
{
private:
    vector<pair<Ptr<GSprite>, tAnimationInfo>> m_vecAnimation;
    
public:
    void AddAnimation(Ptr<GSprite> _Sprite = nullptr, const tAnimationInfo& _Info = tAnimationInfo()) { m_vecAnimation.push_back(make_pair(_Sprite, _Info));}
    void RemoveAnimation(int _Idx);
    void SetAnimation(Ptr<GSprite> _Sprite, const tAnimationInfo& _Info, int _Idx);
    void SetSprite(Ptr<GSprite> _Sprite, int _Idx);
    void SetInfo(const tAnimationInfo& _Info, int _Idx);
    
    int GetAnimationCount() { return (int)m_vecAnimation.size(); }
    Ptr<GSprite> GetSprite(int _Idx) { return m_vecAnimation[_Idx].first; }
    const tAnimationInfo& GetInfo(int _Idx) {return m_vecAnimation[_Idx].second; }
    const pair<Ptr<GSprite>, tAnimationInfo>& GetAnimation(int _Idx) { return m_vecAnimation[_Idx]; }

public:
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;

public:
    CLONE_DELETE(GFlipbook);
public:
    GFlipbook(bool _EngineAsset = false);
    ~GFlipbook();

};

