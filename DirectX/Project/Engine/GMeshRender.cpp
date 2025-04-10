#include "pch.h"
#include "GMeshRender.h"

#include "GGameObject.h"
#include "components.h"

#include "GAssetManager.h"
#include "GMesh.h"
#include "GMaterial.h"

GMeshRender::GMeshRender() 
	: GRenderComponent(COMPONENT_TYPE::MESH_RENDER)
	, m_Color(Vector4(1.f,1.f,1.f,1.f))
{
	// �޽� ����
	if (nullptr == GetMesh())
		SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh"));

	// �޽� ����
	if (nullptr == GetMaterial())
		SetMaterial(GAssetManager::GetInst()->FindAsset<GMaterial>(L"Std2DMtrl"));
}

GMeshRender::~GMeshRender()
{

}

void GMeshRender::FinalUpdate()
{
}

void GMeshRender::Render()
{
	// ī�޶� �̸� �Ÿ��Ƿ� ���� ���� �ʿ䰡 ����.
	/*
	if (!m_Mesh || !m_Shader)
		return;
	*/
	// ��ġ ���� ������Ʈ
	GameObject()->Transform()->Binding();

	// �÷� ����
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Color);

	// ����� ���̴� ���ε�
	GetMaterial()->Binding();

	// ������ ����
	GetMesh()->Render();
}

void GMeshRender::SaveToFile(FILE* _File)
{
	GRenderComponent::SaveToFile(_File);
	fwrite(m_Color, sizeof(Vector4), 1, _File);
}

void GMeshRender::LoadFromFile(FILE* _File)
{
	GRenderComponent::LoadFromFile(_File);
	fread(m_Color, sizeof(Vector4), 1, _File);
}


