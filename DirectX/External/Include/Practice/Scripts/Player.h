#pragma once
#include <Engine/GScript.h>
#include <Engine/GPrefab.h>

class Player :
    public GScript
{
private:
    float m_Speed;
    float m_PaperBurn;

    Ptr<GPrefab> m_Prefab;


public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Update() override;

    virtual void OnTriggerEnter(GCollider2D* _Other);
    virtual void OnTriggerStay(GCollider2D* _Other);
    virtual void OnTriggerExit(GCollider2D* _Other);

    // ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
    virtual void SaveToFile(FILE* _File);
    virtual void LoadFromFile(FILE* _File);

public:
    CLONE(Player);
public:
    Player();
    ~Player();
};

