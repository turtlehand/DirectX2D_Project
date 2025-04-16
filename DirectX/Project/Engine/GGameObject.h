#pragma once
#include "GEntity.h"

class GComponent;
class GScript;
class GRenderComponent;

class GGameObject :
	public GEntity
{
private:
	GGameObject*			m_Parent;									// 부모 오브젝트
	vector<GGameObject*>	m_vecChild;									// 자식 오브젝트

	GComponent*				m_Component[(UINT)COMPONENT_TYPE::END];
	GRenderComponent*		m_RenderComponent;							// RenderComponent는 한개만
	vector<GScript*>		m_vecScript;

	bool					m_Dead;

	int						m_Layer;

public:
	void AddChild(GGameObject* _Child);
	void SetChild(GGameObject* _Child);
	void SetParent(GGameObject* _Parent);
	const vector<GGameObject*>& GetChild() { return m_vecChild; }
	GGameObject* GetParent() { return m_Parent; }
	bool IsAncestor(GGameObject* _Object);

	GComponent* AddComponent(GComponent* _Component);
	GScript* AddComponent(GScript* _Script);
	GComponent* GetComponent(COMPONENT_TYPE _Type) { return m_Component[(UINT)_Type]; }
	const vector<GScript*>& GetScripts() { return m_vecScript; }

	int GetLayer() { return m_Layer; }
	bool IsDead() { return m_Dead; }
	void Destroy();

	template <typename T>
	T* GetComponent();
	template <typename T>
	T* AddComponent();
	template <typename T>
	T* Renderer();

	class GTransform* Transform() { return (GTransform*)GetComponent(COMPONENT_TYPE::TRANSFORM); }
	class GCamera* Camera() { return (GCamera*)GetComponent(COMPONENT_TYPE::CAMERA); }
	class GCollider2D* Collider2D() { return (GCollider2D*)GetComponent(COMPONENT_TYPE::COLLIDER2D); }
	class GLight2D* Light2D() { return (GLight2D*)GetComponent(COMPONENT_TYPE::LIGHT2D); }
	class GRigidBody2D* RigidBody2D() { return (GRigidBody2D*)GetComponent(COMPONENT_TYPE::RIGIDBODY2D); }

	class GRenderComponent* Renderer() { return m_RenderComponent; }
	class GMeshRender* MeshRender() { return (GMeshRender*)GetComponent(COMPONENT_TYPE::MESH_RENDER); }
	class GSpriteRender* SpriteRender() { return (GSpriteRender*)GetComponent(COMPONENT_TYPE::SPRITE_RENDER); }
	class GFlipbookRender* FlipbookRender() { return (GFlipbookRender*)GetComponent(COMPONENT_TYPE::FLIPBOOK_RENDER); }
	class GTileRender* TileRender() { return (GTileRender*)GetComponent(COMPONENT_TYPE::TILE_RENDER); }
	class GParticleRender* ParticleRender() { return (GParticleRender*)GetComponent(COMPONENT_TYPE::PARTICLE_RENDER); }

public:
	void Begin();
	void Update();
	void FinalUpdate();
	void Render();
	void End();

private:
	void DisconnectWithParent();			// 부모와 절연
	void RegisterAsParent();				// 부모로서 정부(Layer)에 등록
	void DisconnectWithLayerAsParent();		// 정부(Layer)에서 독립

public:
	CLONE(GGameObject)

public:
	GGameObject(int Layer = 0);
	GGameObject(const GGameObject& _Origin);
	~GGameObject();
	

	friend class GLayer;
	friend class GTaskManager;
	friend class GPrefab;
};

template<typename T>
inline T* GGameObject::GetComponent()
{
	COMPONENT_TYPE Type = GetComponentType<T>();

	// 스크립트일 시
	if (Type == COMPONENT_TYPE::SCRIPT)
	{
		for (vector<GScript*>::iterator iter = m_vecScript.begin(); iter != m_vecScript.end(); ++iter)
		{
			if (dynamic_cast<T*>((*iter)) != nullptr)
				return (T*)(*iter);
		}
	}
	else
	{
		return (T*)m_Component[(UINT)Type];
	}



	return nullptr;
}

template<typename T>
inline T* GGameObject::AddComponent()
{
	T* Component = new T;

	// 스크립트 컴포넌트일 경우
	if (Component->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((GScript*)Component);
	}
	else
	{
		// 이미 가지고 있는 컴포넌트인 경우
		assert(!m_Component[(UINT)Component->GetType()]);
		m_Component[(UINT)Component->GetType()] = Component;

		// Render Component 일 경우
		if (dynamic_cast<GRenderComponent*>(Component) != nullptr)
		{
			// 하나의 GameObject는 한 종류의 RenderComponent만 가질 수 있다.
			if (nullptr != m_RenderComponent)
				assert(nullptr);

			m_RenderComponent = (GRenderComponent*)Component;
		}
			
	}

	Component->m_GameObject = this;
	// 복사생성해서 생성된 컴포넌트 초기화
	Component->Init();

	return Component;
}

template <typename T>
inline T* GGameObject::Renderer()
{
	COMPONENT_TYPE Type = GetComponentType<T>();

	// 해당 컴포넌트 타입이 Render에 속해있다면
	assert((int)COMPONENT_TYPE::MESH_RENDER <= (int)Type && (int)Type < (int)COMPONENT_TYPE::END);

	return (T*)m_RenderComponent;
}