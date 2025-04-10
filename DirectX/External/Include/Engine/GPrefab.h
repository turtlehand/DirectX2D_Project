#pragma once
#include "GAsset.h"

// 1. �۾��� ���꼺 �ø���
// 2. ���� �÷��� ���� �ε��� ���� �� �ִ�.
//      - ��ü�� ���� ������ �� �߻��ϴ� ���ҽ� �ε��� �̸� �����ص�

typedef void(*GAMEOBJECT_SAVE)(GGameObject*, FILE*);
typedef GGameObject* (*GAMEOBJECT_LOAD)(FILE*);

class GPrefab :
	public GAsset
{
public:
	static GAMEOBJECT_SAVE g_Save;
	static GAMEOBJECT_LOAD g_Load;
private:
	GGameObject* m_PrefabObject;

public:
	void SetGameObject(GGameObject* _Object);       // �θ� ���� ������Ʈ�� �ֻ��� ������Ʈ�� ����
	GGameObject* Instantiate();

public:
	virtual int Save(const wstring& _FilePath) override;
	virtual int Load(const wstring& _FilePath) override;

public:
	CLONE_DELETE(GPrefab);
	
public:
	GPrefab(bool _EngineAsset = false);
	~GPrefab();
};

