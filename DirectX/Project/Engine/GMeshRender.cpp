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
	// 메쉬 설정
	if (nullptr == GetMesh())
		SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh"));

	// 메쉬 설정
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
	// 카메라가 미리 거르므로 굳이 있을 필요가 없다.
	/*
	if (!m_Mesh || !m_Shader)
		return;
	*/
	// 위치 정보 업데이트
	GameObject()->Transform()->Binding();

	// 컬러 적용
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Color);

	// 사용할 쉐이더 바인딩
	GetMaterial()->Binding();

	// 렌더링 시작
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


