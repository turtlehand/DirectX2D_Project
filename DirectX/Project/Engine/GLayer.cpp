#include "pch.h"
#include "GLayer.h"

#include "GGameObject.h"

GLayer::GLayer() :
	m_LayerIdx(-1)
{
}

GLayer::GLayer(const GLayer& _Origin)
	: GEntity(_Origin)
	, m_LayerIdx(_Origin.m_LayerIdx)
{
	for (size_t i = 0;i < _Origin.m_vecParentObjects.size();++i)
	{
		GGameObject* pObject = _Origin.m_vecParentObjects[i]->Clone();
		AddGameObject(pObject,false);
	}
}

GLayer::~GLayer()
{
	Delete_Vec(m_vecParentObjects);
}

GGameObject* GLayer::FindObject(const wstring& _Name)
{
	for (int i = 0; i < m_vecObjects.size(); ++i)
	{
		if (m_vecObjects[i]->GetName() == _Name)
			return m_vecObjects[i];
	}

	return nullptr;
}

void GLayer::Begin()
{
	for (size_t i = 0; i < m_vecParentObjects.size(); ++i)
	{
		m_vecParentObjects[i]->Begin();
	}
}

void GLayer::Update()
{
	for (size_t i = 0; i < m_vecParentObjects.size(); ++i)
	{
		m_vecParentObjects[i]->Update();
	}
}

void GLayer::FinalUpdate()
{
	vector<GGameObject*>::iterator iter = m_vecParentObjects.begin();

	for (; iter != m_vecParentObjects.end(); )
	{
		(*iter)->FinalUpdate();

		if ((*iter)->IsDead())
		{
			iter = m_vecParentObjects.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

// 레이어에 최상위 부모 오브젝트를 추가한다.
// _bChildMove가 True면 부모와 같은 레이어를 가진다.
// _bChildMove가 False라면 -1이 아니라면 유지한다.
void GLayer::AddGameObject(GGameObject* _Parent, bool _bChildMove)
{
	m_vecParentObjects.push_back(_Parent);

	// 자식을 보유했을 경우, 자식도 해당 레리어 소속으로 변경한다.
	static list<GGameObject*> queue;
	queue.clear();
	queue.push_back(_Parent);

	while (!queue.empty())
	{
		GGameObject* pObject = queue.front();
		queue.pop_front();

		const vector<GGameObject*>& pChild = pObject->GetChild();

		for (size_t i = 0; i < pChild.size(); ++i)
		{
			queue.push_back(pChild[i]);
		}

		// 최상위 부모 오브젝트 
		// or 자식오브젝트여도, 자식까지 같이 이동하기로 한 경우 
		// or 최상위부모오브젝트x, 자식이동x 더라도 소속이 없는 오브젝트인 경우
		if (nullptr == pObject->GetParent() || _bChildMove || pObject->m_Layer == -1)
		{
			pObject->m_Layer = m_LayerIdx;
		}
	}
}

void GLayer::DeRegisterGameObject(GGameObject* _Object)
{

	vector<GGameObject*>::iterator iter = m_vecParentObjects.begin();

	for (; iter != m_vecParentObjects.end(); ++iter)
	{
		if ((*iter) == _Object)
		{
			m_vecParentObjects.erase(iter);
			return;
		}
	}

	// 현재 레이어에 해당 오브젝트가 없는 경우
	assert(nullptr);
}
