#include "pch.h"
#include "TransformUI.h"

#include <Engine/components.h>

TransformUI::TransformUI()
	:ComponentUI(COMPONENT_TYPE::TRANSFORM)
{

}

TransformUI::~TransformUI()
{
}

void TransformUI::Render_UI()
{
	if (!IsValid(GetTarget()) || GetTarget()->Transform() == nullptr)
	{
		SetActive(false);
		return;
	}

	GTransform* pTrans = GetTarget()->Transform();

	OutputTitle("Transform");

	Vector3 vRelativePos = pTrans->GetRelativePos();
	Vector3 vRelativeScale = pTrans->GetRelativeScale();
	Vector3 vRelativeRotation = pTrans->GetRelativeRotation();

	ImGui::Text("Trans");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Translation", vRelativePos))
	{
		pTrans->SetRelativePos(vRelativePos);
	}
	AddItemHeight();

	ImGui::Text("Scale");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Scale", vRelativeScale))
	{
		pTrans->SetRelativeScale(vRelativeScale);
	}
	AddItemHeight();

	ImGui::Text("Roatation");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Rotation", vRelativeRotation))
	{
		pTrans->SetRelativeRotation(vRelativeRotation);
	}
	AddItemHeight();
}


