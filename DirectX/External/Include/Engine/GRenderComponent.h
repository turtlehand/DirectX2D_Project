#pragma once
#include "GComponent.h"

#include "GMesh.h"
#include "GMaterial.h"

class GRenderComponent :
	public GComponent
{
private:
	Ptr<GMesh>      m_Mesh;

	Ptr<GMaterial>  m_SharedMtrl;	// AssetManager로부터 얻어옴
	Ptr<GMaterial>  m_CurMtrl;		// RenderComponent 본인이 현재 사용중인 재질

	// RenderComponent 객체 본인만 사용할 전용 재질(AssetManager에 등록시키지 않는다.)
	// 동적 재질 생성은 현재 레벨의 상태가 Play 상태일 경우에만 사용 가능
	Ptr<GMaterial>  m_DynamicMtrl;

public:
	void SetMesh(Ptr<GMesh> _Mesh) { m_Mesh = _Mesh; }
	Ptr<GMesh> GetMesh() { return m_Mesh; }

	void SetMaterial(Ptr<GMaterial> _Material);
	Ptr<GMaterial> GetMaterial() { return m_CurMtrl; }
	Ptr<GMaterial> GetSharedMaterial() { m_CurMtrl = m_SharedMtrl; return m_SharedMtrl; }
	Ptr<GMaterial> GetDynamicMaterial();

	virtual GRenderComponent* Clone() = 0;

public:
	virtual void Render() = 0;

	// 컴포넌트 정보가 파일(레벨)에 저장 / 불러올 때 필수로 저장해야 하는 내용을 작성
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	GRenderComponent(COMPONENT_TYPE _Type);
	GRenderComponent(const GRenderComponent& _Origin);
	~GRenderComponent();
};

