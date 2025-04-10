#pragma once
#include "GComponent.h"

#include "GMesh.h"
#include "GMaterial.h"

class GRenderComponent :
	public GComponent
{
private:
	Ptr<GMesh>      m_Mesh;

	Ptr<GMaterial>  m_SharedMtrl;	// AssetManager�κ��� ����
	Ptr<GMaterial>  m_CurMtrl;		// RenderComponent ������ ���� ������� ����

	// RenderComponent ��ü ���θ� ����� ���� ����(AssetManager�� ��Ͻ�Ű�� �ʴ´�.)
	// ���� ���� ������ ���� ������ ���°� Play ������ ��쿡�� ��� ����
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

	// ������Ʈ ������ ����(����)�� ���� / �ҷ��� �� �ʼ��� �����ؾ� �ϴ� ������ �ۼ�
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);

public:
	GRenderComponent(COMPONENT_TYPE _Type);
	GRenderComponent(const GRenderComponent& _Origin);
	~GRenderComponent();
};

