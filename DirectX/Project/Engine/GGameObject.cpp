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
	// 컴포넌트 복제
	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;++i)
	{
		if (nullptr == _Origin.m_Component[i])
			continue;
		AddComponent(_Origin.m_Component[i]->Clone());
	}

	// 스크립트 복제
	for (UINT i = 0;i < _Origin.m_vecScript.size();++i)
	{
		AddComponent(_Origin.m_vecScript[i]->Clone());
	}

	// 자식 오브젝트 복제
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
		// 조상이 맞다.
		if (pAncestor == _Object)
			return true;

		pAncestor = pAncestor->GetParent();
	}

	// 조상이 아니다.
	return false;
}

GComponent* GGameObject::AddComponent(GComponent* _Component)
{
	assert(_Component);
	// 스크립트 컴포넌트일 경우
	if (_Component->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((GScript*)_Component);
	}
	else
	{
		// 이미 가지고 있는 컴포넌트인 경우
		assert(!m_Component[(UINT)_Component->GetType()]);

		m_Component[(UINT)_Component->GetType()] = _Component;

		// Render Component 일 경우
		if (dynamic_cast<GRenderComponent*>(_Component) != nullptr)
		{
			// 하나의 GameObject는 한 종류의 RenderComponent만 가질 수 있다.
			if (nullptr != m_RenderComponent)
				assert(nullptr);

			m_RenderComponent = (GRenderComponent*)_Component;
		}

	}

	_Component->m_GameObject = this;
	// 복사생성해서 생성된 컴포넌트 초기화
	_Component->Init();

	return _Component;
}

GScript* GGameObject::AddComponent(GScript* _Script)
{
	assert(_Script);
	// 스크립트 컴포넌트일 경우
	m_vecScript.push_back((GScript*)_Script);


	_Script->m_GameObject = this;
	// 복사생성해서 생성된 컴포넌트 초기화
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
	// 스크립트 Update 시켜주기
	vector<GScript*>::iterator iter = m_vecScript.begin();
	for (; iter != m_vecScript.end(); )
	{
		(*iter)->Update();

		// 스크립트가 곧 삭제될거라면 벡터에서 제거시켜준다.
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
	// Layer에 GameObject 등록하기
	// 컴포넌트가 삭제되든 말든 등록할 때는 상관없음
	GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();
	GLayer* pMyLayer = pCurLevel->GetLayer(m_Layer);
	pMyLayer->RegisterGameObject(this);

	for (UINT i = 0;i < (UINT)COMPONENT_TYPE::END;++i)
	{
		if (nullptr == m_Component[i])
			continue;
		m_Component[i]->FinalUpdate();

		// 정적 배열이므로 단순 반복문을 써도 된다.
		if (m_Component[i]->IsDead())
		{
			m_Component[i] = nullptr;
			// Render Component 일 경우
			if ( (UINT)COMPONENT_TYPE::MESH_RENDER <= i && (UINT)COMPONENT_TYPE::END)
			{
				m_RenderComponent = nullptr;
			}
		}
	}


	// 자식들 FinalUpdate 시켜주기
	vector<GGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalUpdate();

		// 자식이 삭제될 거라면 자식 벡터에서 제거한다.
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
		// 없거나 이미 죽은 대상이라면 End를 호출시키지 않는다.
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
	// 부모가 있어야 가능
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

	// 자식이 부모로 지정한 오브젝트가 자식 오브젝트의 주소를 모르는 경우
	assert(nullptr);

}

void GGameObject::RegisterAsParent()
{
	// 부모가 없어야 가능
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


