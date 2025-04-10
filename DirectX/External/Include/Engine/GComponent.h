#pragma once
#include "GEntity.h"
#include "GGameObject.h"

#define GET_OTHER_COMPONENT(Type) class G##Type* Type() { return m_GameObject->Type(); }

class GComponent :
	public GEntity
{
private:
	const COMPONENT_TYPE m_Type;
	GGameObject* m_GameObject;

	bool m_Dead;

public:
	COMPONENT_TYPE GetType() const { return m_Type; }
	bool IsDead() { return m_Dead; }
	void Destroy();

	class GGameObject* GameObject() const { return m_GameObject; }

public:
	GET_OTHER_COMPONENT(Transform);
	GET_OTHER_COMPONENT(Camera);
	GET_OTHER_COMPONENT(Collider2D);
	GET_OTHER_COMPONENT(Light2D);
	GET_OTHER_COMPONENT(RigidBody2D);

	class GRenderComponent* Renderer() { return m_GameObject->Renderer(); }
	GET_OTHER_COMPONENT(MeshRender);
	GET_OTHER_COMPONENT(SpriteRender);
	GET_OTHER_COMPONENT(FlipbookRender);
	GET_OTHER_COMPONENT(TileRender);
	GET_OTHER_COMPONENT(ParticleRender);


public:
	virtual void Init() {};
	virtual void Begin() {};
	virtual void FinalUpdate() = 0;
	virtual void Render() {};
	virtual void End() {};

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File) = 0;
	virtual void LoadFromFile(FILE* _File) = 0;

public:
	virtual GComponent* Clone() = 0;
	
public:
	GComponent(COMPONENT_TYPE _Type);
	GComponent(const GComponent& _Origin);
	virtual ~GComponent();

	friend class GTaskManager;
	friend class GGameObject;
};

