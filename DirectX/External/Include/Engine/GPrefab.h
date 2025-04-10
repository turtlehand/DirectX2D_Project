#pragma once
#include "GAsset.h"

// 1. 작업의 생산성 올린다
// 2. 게임 플레이 도중 로딩을 줄일 수 있다.
//      - 객체가 최초 등장할 때 발생하는 리소스 로딩을 미리 수행해둠

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
	void SetGameObject(GGameObject* _Object);       // 부모가 없는 오브젝트만 최상위 오브젝트만 가능
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

