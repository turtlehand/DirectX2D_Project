#pragma once
#include "GEntity.h"

class GComponent;
class GScript;
class GRenderComponent;

class GGameObject :
	public GEntity
{
private:
	GGameObject*			m_Parent;									// �θ� ������Ʈ
	vector<GGameObject*>	m_vecChild;									// �ڽ� ������Ʈ

	GComponent*				m_Component[(UINT)COMPONENT_TYPE::END];
	GRenderComponent*		m_RenderComponent;							// RenderComponent�� �Ѱ���
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
	void DisconnectWithParent();			// �θ�� ����
	void RegisterAsParent();				// �θ�μ� ����(Layer)�� ���
	void DisconnectWithLayerAsParent();		// ����(Layer)���� ����

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

	// ��ũ��Ʈ�� ��
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

	// ��ũ��Ʈ ������Ʈ�� ���
	if (Component->GetType() == COMPONENT_TYPE::SCRIPT)
	{
		m_vecScript.push_back((GScript*)Component);
	}
	else
	{
		// �̹� ������ �ִ� ������Ʈ�� ���
		assert(!m_Component[(UINT)Component->GetType()]);
		m_Component[(UINT)Component->GetType()] = Component;

		// Render Component �� ���
		if (dynamic_cast<GRenderComponent*>(Component) != nullptr)
		{
			// �ϳ��� GameObject�� �� ������ RenderComponent�� ���� �� �ִ�.
			if (nullptr != m_RenderComponent)
				assert(nullptr);

			m_RenderComponent = (GRenderComponent*)Component;
		}
			
	}

	Component->m_GameObject = this;
	// ��������ؼ� ������ ������Ʈ �ʱ�ȭ
	Component->Init();

	return Component;
}

template <typename T>
inline T* GGameObject::Renderer()
{
	COMPONENT_TYPE Type = GetComponentType<T>();

	// �ش� ������Ʈ Ÿ���� Render�� �����ִٸ�
	assert((int)COMPONENT_TYPE::MESH_RENDER <= (int)Type && (int)Type < (int)COMPONENT_TYPE::END);

	return (T*)m_RenderComponent;
}