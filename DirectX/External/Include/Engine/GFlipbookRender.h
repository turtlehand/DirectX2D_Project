#pragma once
#include "GRenderComponent.h"

#include "GFlipbook.h"

class GFlipbookRender :
    public GRenderComponent
{
private:
    vector<Ptr<GFlipbook>>  m_vecFlipbook;
    int                     m_CurFlipbookIndex;
    int                     m_SpriteIdx;
    float                   m_Time;
    bool                    m_Repeat;
    bool                    m_Finish;

    bool                    m_Pause;

public:
    int GetFlipbookCount() { return m_vecFlipbook.size(); }

    void AddFlipBook(Ptr<GFlipbook> _Flipbook);
    void SetFlipbook(Ptr<GFlipbook> _Flipbook, int _Idx);
    void RemoveFlipbbok(int _Idx);

    Ptr<GFlipbook> GetCurFlipbook() { return m_vecFlipbook[m_CurFlipbookIndex]; }
    int GetCurIndex() { return m_CurFlipbookIndex; }
    Ptr<GFlipbook> GetFlipbook(int _Idx) { return m_vecFlipbook[_Idx]; }

    bool GetRepeat() { return m_Repeat; }
    void SetRepeat(bool _Repeat) { m_Repeat = _Repeat; }
    bool GetFinish() { return m_Finish; }

    void Play(int _Idx, bool _Repeat = true)
    {
        m_CurFlipbookIndex = _Idx;
        m_Time = 0.f;
        m_Finish = false;
        m_Repeat = _Repeat;
        m_SpriteIdx = 0;
        m_Pause = false;
    }

    bool GetPause()
    {
        return m_Pause;
    }

    void Pause()
    {
        m_Pause = !m_Pause;
    }

public:
    virtual void FinalUpdate() override;
    virtual void Render() override;


    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

private:
    void CreateFlipbookMaterial();
public:
    CLONE(GFlipbookRender);
    GFlipbookRender();
    ~GFlipbookRender();

};

