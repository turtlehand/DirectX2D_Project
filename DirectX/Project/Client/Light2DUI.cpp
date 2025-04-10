#include "pch.h"
#include "Light2DUI.h"

#include <Engine/GLight2D.h>

Light2DUI::Light2DUI()
	:ComponentUI(COMPONENT_TYPE::LIGHT2D)
{

}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::Render_UI()
{
	if (!IsValid(GetTarget()) || GetTarget()->Light2D() == nullptr)
	{
		SetActive(false);
		return;
	}
	GLight2D* pLight2D = (GLight2D*)GetTarget()->GetComponent(COMPONENT_TYPE::LIGHT2D);

	OutputTitle("Light2D");

	// ±¤¿ø Å¸ÀÔ
	ImGui::Text("Light Type");
	ImGui::SameLine(GetTab());

	const char* items[] = { "DIRECTIONAL", "POINT", "SPOT"};
	int type = (int)pLight2D->GetLightType();
	ImGui::Combo("##Light Combo", &type, items, IM_ARRAYSIZE(items));

	if (type != (int)pLight2D->GetLightType())
		pLight2D->SetLightType((LIGHT_TYPE)type);
	AddItemHeight();

	// Radius
	ImGui::BeginDisabled(LIGHT_TYPE::DIRECTIONAL == pLight2D->GetLightType());
	{
		float Radius = pLight2D->GetRadius();
		ImGui::Text("Radius");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Radius", &Radius))
		{
			pLight2D->SetRadius(Radius);
		}
	}
	ImGui::EndDisabled();
	AddItemHeight();

	// Angle
	ImGui::BeginDisabled(LIGHT_TYPE::SPOT != pLight2D->GetLightType());
	{
		float Angle = pLight2D->GetDegree();
		ImGui::Text("Degree");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Degree", &Angle))
		{
			pLight2D->SetDegree(Angle);
		}
	}
	ImGui::EndDisabled();
	AddItemHeight();

	// LightColor
	Vector3 vColor = pLight2D->GetLightColor();
	ImGui::Text("Light Color");
	ImGui::SameLine(GetTab());

	if (ImGui::ColorEdit3("##vColor", vColor))
	{
		pLight2D->SetLightColor(vColor);
	}
	AddItemHeight();

}


