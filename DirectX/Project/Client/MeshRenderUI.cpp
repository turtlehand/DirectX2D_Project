#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/GMeshRender.h>

MeshRenderUI::MeshRenderUI()
	: RenderComponentUI(COMPONENT_TYPE::MESH_RENDER)
	, m_MeshRender(nullptr)
{

}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::Render_UI()
{
	if (!IsValid(GetTarget()) || GetTarget()->MeshRender() == nullptr)
	{
		SetActive(false);
		return;
	}
	m_MeshRender = GetTarget()->MeshRender();
	OutputTitle("Mesh Render");
	RenderComponentUI::Render_UI();

	// LightColor
	Vector4 vColor = m_MeshRender->GetColor();
	ImGui::Text("Color");
	ImGui::SameLine(GetTab());
	if (ImGui::ColorEdit4("##vColor", vColor))
	{
		m_MeshRender->SetColor(vColor);
	}
	AddItemHeight();
}


