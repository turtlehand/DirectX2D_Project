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

// ���̾ �ֻ��� �θ� ������Ʈ�� �߰��Ѵ�.
// _bChildMove�� True�� �θ�� ���� ���̾ ������.
// _bChildMove�� False��� -1�� �ƴ϶�� �����Ѵ�.
void GLayer::AddGameObject(GGameObject* _Parent, bool _bChildMove)
{
	m_vecParentObjects.push_back(_Parent);

	// �ڽ��� �������� ���, �ڽĵ� �ش� ������ �Ҽ����� �����Ѵ�.
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

		// �ֻ��� �θ� ������Ʈ 
		// or �ڽĿ�����Ʈ����, �ڽı��� ���� �̵��ϱ�� �� ��� 
		// or �ֻ����θ������Ʈx, �ڽ��̵�x ���� �Ҽ��� ���� ������Ʈ�� ���
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

	// ���� ���̾ �ش� ������Ʈ�� ���� ���
	assert(nullptr);
}
