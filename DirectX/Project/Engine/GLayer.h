#pragma once
#include "GEntity.h"

class GGameObject;
class GCollider2D;

class GLayer :
	public GEntity
{
private:
	vector<GGameObject*>    m_vecParentObjects; // ���̾ �Ҽӵ� ������Ʈ ��, �ֻ��� �θ� ������Ʈ�� ����
	vector<GGameObject*>    m_vecObjects;       // �ֻ��� �θ�, �ڽ� �������, �ش� ���̾� �Ҽ� ��� ������Ʈ
	int                     m_LayerIdx;         // ���̾� ��ȣ

	vector<GCollider2D*>	m_vecCollider2D;

public:
	
	void AddGameObject(GGameObject* _Parent, bool _bChildMove = false);					// �ֻ��� ������Ʈ�� ���̾ ����Ѵ�.
		
	void RegisterGameObject(GGameObject* _Object) { m_vecObjects.push_back(_Object); }	// ���̾��� ������Ʈ�� ����Ѵ�.

	void DeRegisterGameObject(GGameObject* _Object);									// ���̾�� ������Ʈ�� �����Ѵ�.
	
	const vector<GGameObject*>& GetParentObjects() { return m_vecParentObjects; }		// �ֻ��� ������Ʈ�� ��ȯ�Ѵ�.

	const vector<GGameObject*>& GetObjects() { return m_vecObjects; }					// �ش� ���̾��� ��� ������Ʈ�� ��ȯ�Ѵ�.  

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

