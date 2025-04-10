#include "pch.h"
#include "GGameObject.h"
#include "components.h"

#include "GScript.h"

#include "GTaskManager.h"
#include "GLevelManager.h"
#include "GLevel.h"
#include "GLayer.h"

GGameObject::GGameObject(int Layer) :
	GEntity(),
	m_Component{},
	m_RenderComponent(nullptr),
	m_Parent(nullptr),
	m_Layer(Layer),
	m_Dead(false)
{
}

GGameObject::~GGameObject()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_Component[i])
			delete m_Component[i];
	}

	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

GGameObject::GGameObject(const GGameObject& _Origin) :
	GEntity(_Origin),
	m_Component{},
	m_RenderComponent(nullptr),
	m_Parent(nullptr),
	m_Layer(_Origin.m_Layer),
	m_Dead(false)
{
	// ������Ʈ ����
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;++i)
	{
		if (nullptr == _Origin.m_Component[i])
			continue;
		AddComponent(_Origin.m_Component[i]->Clone());
	}

	// ��ũ��Ʈ ����
	for (UINT i = 0;i < _Origin.m_vecScript.size();++i)
	{
		AddComponent(_Origin.m_vecScript[i]->Clone());
	}

	// �ڽ� ������Ʈ ����
	for (UINT i = 0;i < _Origin.m_vecChild.size();++i)
	{
		AddChild(_Origin.m_vecChild[i]->Clone());
	}
}

void GGameObject::AddChild(GGameObject* _Child)
{
	m_vecChild.push_back(_Child);
	_Child->m_Parent = this;
}

bool GGameObject::IsAncestor(GGameObject* _Object)
{
	GGameObject* pAncestor = GetParent();

	while (pAncestor != nullptr)
	{
		// ������ �´�.
		if (pAncestor == _Object)
			return true;

		pAncestor = pAncestor->GetParent();
	}

	// ������ �ƴϴ�.
	return false;
}

GComponent* GGameObject::AddComponent(GComponent* _Component)
{
	assert(_Component);
	// ��ũ��Ʈ ������Ʈ�� ���
	if (_Component->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((GScript*)_Component);
	}
	else
	{
		// �̹� ������ �ִ� ������Ʈ�� ���
		assert(!m_Component[(UINT)_Component->GetType()]);

		m_Component[(UINT)_Component->GetType()] = _Component;

		// Render Component �� ���
		if (dynamic_cast<GRenderComponent*>(_Component) != nullptr)
		{
			// �ϳ��� GameObject�� �� ������ RenderComponent�� ���� �� �ִ�.
			if (nullptr != m_RenderComponent)
				assert(nullptr);

			m_RenderComponent = (GRenderComponent*)_Component;
		}

	}

	_Component->m_GameObject = this;
	// ��������ؼ� ������ ������Ʈ �ʱ�ȭ
	_Component->Init();

	return _Component;
}

GScript* GGameObject::AddComponent(GScript* _Script)
{
	assert(_Script);
	// ��ũ��Ʈ ������Ʈ�� ���
	m_vecScript.push_back((GScript*)_Script);


	_Script->m_GameObject = this;
	// ��������ؼ� ������ ������Ʈ �ʱ�ȭ
	_Script->Init();

	return _Script;
}

void GGameObject::Destroy()
{
	tTask task = { TASK_TYPE::DESTROY_OBJECT, (DWORD_PTR)this };

	GTaskManager::GetInst()->AddTask(task);
}

void GGameObject::Begin()
{
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;++i)
	{
		if (nullptr != m_Component[i])
			m_Component[i]->Begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}

}

void GGameObject::Update()
{
	// ��ũ��Ʈ Update �����ֱ�
	vector<GScript*>::iterator iter = m_vecScript.begin();
	for (; iter != m_vecScript.end(); )
	{
		(*iter)->Update();

		// ��ũ��Ʈ�� �� �����ɰŶ�� ���Ϳ��� ���Ž����ش�.
		if ((*iter)->IsDead())
			iter = m_vecScript.erase(iter);
		else
			++iter;
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Update();
	}
}

void GGameObject::FinalUpdate()
{
	// Layer�� GameObject ����ϱ�
	// ������Ʈ�� �����ǵ� ���� ����� ���� �������
	GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();
	GLayer* pMyLayer = pCurLevel->GetLayer(m_Layer);
	pMyLayer->RegisterGameObject(this);

	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;++i)
	{
		if (nullptr == m_Component[i])
			continue;
		m_Component[i]->FinalUpdate();

		// ���� �迭�̹Ƿ� �ܼ� �ݺ����� �ᵵ �ȴ�.
		if (m_Component[i]->IsDead())
		{
			m_Component[i] = nullptr;
			// Render Component �� ���
			if ( (UINT)COMPONENT_TYPE::MESH_RENDER <= i && (UINT)COMPONENT_TYPE::END)
			{
				m_RenderComponent = nullptr;
			}
		}
	}


	// �ڽĵ� FinalUpdate �����ֱ�
	vector<GGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalUpdate();

		// �ڽ��� ������ �Ŷ�� �ڽ� ���Ϳ��� �����Ѵ�.
		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}

}

void GGameObject::Render()
{
	if (!m_RenderComponent)
		return;
	m_RenderComponent->Render();
}

void GGameObject::End()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		// ���ų� �̹� ���� ����̶�� End�� ȣ���Ű�� �ʴ´�.
		if (nullptr == m_Component[i] || m_Component[i]->IsDead())
			continue;
		m_Component[i]->End();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsDead())
			continue;
		m_vecScript[i]->End();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsDead())
			continue;
		m_vecChild[i]->End();
	}
}

void GGameObject::DisconnectWithParent()
{
	// �θ� �־�� ����
	assert(m_Parent);

	vector<GGameObject*>::iterator iter = m_Parent->m_vecChild.begin();

	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if ((*iter) == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	// �ڽ��� �θ�� ������ ������Ʈ�� �ڽ� ������Ʈ�� �ּҸ� �𸣴� ���
	assert(nullptr);

}

void GGameObject::RegisterAsParent()
{
	// �θ� ����� ����
	assert(!m_Parent);

	GLevel* pLevel = GLevelManager::GetInst()->GetCurrentLevel();
	GLayer* pCurLayer = pLevel->GetLayer(m_Layer);
	pCurLayer->AddGameObject(this);
}

void GGameObject::DisconnectWithLayerAsParent()
{
	GLevel* pLevel = GLevelManager::GetInst()->GetCurrentLevel();
	GLayer* pCurLayer = pLevel->GetLayer(m_Layer);
	pCurLayer->DeRegisterGameObject(this);
}


