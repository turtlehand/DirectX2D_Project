#include "pch.h"
#include "GLevel.h"

#include "GRenderManager.h"

#include "GLayer.h"
#include "GGameObject.h"

GLevel::GLevel()
	: GEntity()
	, m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = new GLayer;
		m_Layer[i]->m_LayerIdx = i;
	}
}

GLevel::GLevel(const GLevel& _Origin)
	: GEntity(_Origin)
	, m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = _Origin.m_Layer[i]->Clone();
	}
}

GLevel::~GLevel()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		delete m_Layer[i];
	}
}

void GLevel::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Begin();
	}
}

void GLevel::Update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Update();
	}
}

void GLevel::FinalUpdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->FinalUpdate();
	}
}

// ������ ������Ʈ�� �߰��Ѵ�.
// _bCildMove�� True�� �ڽĿ�����Ʈ�� ���̾ �̵��Ѵ�.
void GLevel::AddGameObject(GGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	m_Layer[_LayerIdx]->AddGameObject(_Object, _bChildMove);
}

void GLevel::ChangeState(LEVEL_STATE _NextState)
{
	// ���� ���¿�, ������ ���°� ������ ���
	if (m_State == _NextState)
		return;

	// Stop -> Pause (X)
	if (m_State == LEVEL_STATE::STOP && _NextState == LEVEL_STATE::PAUSE)
		assert(nullptr);

	// Play -> Pasue
	// Stop -> Play
	// pause -> Play
	if (m_State == LEVEL_STATE::STOP && _NextState == LEVEL_STATE::PLAY)
	{
		m_State = _NextState;
		Begin();
	}
	else
	{
		m_State = _NextState;
	}

	if (LEVEL_STATE::PLAY == m_State)
		GRenderManager::GetInst()->SetRenderMode(true);
	else
		GRenderManager::GetInst()->SetRenderMode(false);
}

GGameObject* GLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<GGameObject*>& vecObj = m_Layer[i]->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetName() == _Name)
			{
				return vecObj[j];
			}
		}
	}

	return nullptr;
}

// ���̾ �ʱ�ȭ�Ѵ�.
void GLevel::Deregister()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->m_vecObjects.clear();
		m_Layer[i]->m_vecCollider2D.clear();
	}
}
