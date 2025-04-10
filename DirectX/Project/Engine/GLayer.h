#pragma once
#include "GEntity.h"

class GGameObject;
class GCollider2D;

class GLayer :
	public GEntity
{
private:
	vector<GGameObject*>    m_vecParentObjects; // 레이어에 소속된 오브젝트 중, 최상위 부모 오브젝트만 관리
	vector<GGameObject*>    m_vecObjects;       // 최상위 부모, 자식 상관없이, 해당 레이어 소속 모든 오브젝트
	int                     m_LayerIdx;         // 레이어 번호

	vector<GCollider2D*>	m_vecCollider2D;

public:
	
	void AddGameObject(GGameObject* _Parent, bool _bChildMove = false);					// 최상위 오브젝트를 레이어에 등록한다.
		
	void RegisterGameObject(GGameObject* _Object) { m_vecObjects.push_back(_Object); }	// 레이어의 오브젝트를 등록한다.

	void DeRegisterGameObject(GGameObject* _Object);									// 레이어에서 오브젝트를 제거한다.
	
	const vector<GGameObject*>& GetParentObjects() { return m_vecParentObjects; }		// 최상위 오브젝트만 반환한다.

	const vector<GGameObject*>& GetObjects() { return m_vecObjects; }					// 해당 레이어의 모든 오브젝트를 반환한다.  

	void RegisterCollider2D(GCollider2D* _Collider) { m_vecCollider2D.push_back(_Collider); }

	const vector<GCollider2D*>& GetCollider2D() { return m_vecCollider2D; }

public:
	void Begin();
	void Update();
	void FinalUpdate();

public:
	CLONE(GLayer);

public:
	GLayer();
	~GLayer();
private:
	GLayer(const GLayer& _Origin);
	friend class GLevel;
};

