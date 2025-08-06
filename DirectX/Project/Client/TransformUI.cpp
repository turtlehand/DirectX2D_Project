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

	ImGui::Text("Pos");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Translation", vRelativePos, 0.1f))
	{
		pTrans->SetRelativePos(vRelativePos);
	}
	AddItemHeight();

	ImGui::Text("Scale");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Scale", vRelativeScale, 0.1f))
	{
		pTrans->SetRelativeScale(vRelativeScale);
	}
	AddItemHeight();

	ImGui::Text("Roatation");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Rotation", vRelativeRotation, 0.1f))
	{
		pTrans->SetRelativeRotation(vRelativeRotation);
	}
	AddItemHeight();
}


