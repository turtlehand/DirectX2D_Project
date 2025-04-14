#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/GCollider2D.h>

Collider2DUI::Collider2DUI()
	:ComponentUI(COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Render_UI()
{
	if (!IsValid(GetTarget()) || GetTarget()->Collider2D() == nullptr)
	{
		SetActive(false);
		return;
	}
	GCollider2D* pCollider2D = (GCollider2D*)GetTarget()->GetComponent(COMPONENT_TYPE::COLLIDER2D);

	OutputTitle("GCollider2D");

	ImGui::Text("Sensor");
	ImGui::SameLine(GetTab());
	bool IsSensor = pCollider2D->IsSensor();
	if(ImGui::Checkbox("##Sensor", &IsSensor))
	{
		pCollider2D->SetSensor(IsSensor);
	}
	AddItemHeight();

	ImGui::Text("Pos");
	ImGui::SameLine(GetTab());
	Vector2 vPos = pCollider2D->GetPos();
	ImGui::DragFloat2("##Pos", vPos);

	AddItemHeight();

	
	// Å©±â
	ImGui::Text("Scale");
	ImGui::SameLine(GetTab());
	Vector2 vScale = pCollider2D->GetScale();
	if (ImGui::DragFloat2("##Scale", vScale, 0.1f, 0.01f, FLT_MAX))
	{
		pCollider2D->SetScale(vScale);
	}
	AddItemHeight();

	/*
	ImGui::Text("Offset");
	ImGui::SameLine(GetTab());
	Vector3 vOffset = pCollider2D->GetOffset();
	if (ImGui::DragFloat2("##Offset", vOffset))
	{
		pCollider2D->SetOffset(vOffset);
	}
	AddItemHeight();

	ImGui::Text("NotifyParent");
	ImGui::SameLine(GetTab());
	bool bNotify = pCollider2D->GetNotifyParent();
	if (ImGui::Checkbox("##NotifyParent",&bNotify))
	{
		pCollider2D->SetNotifyParent(bNotify);
	}
	AddItemHeight();
	*/
}