#include "pch.h"
#include "RigidBody2DUI.h"

#include <Engine/GRigidBody2D.h>

RigidBody2DUI::RigidBody2DUI()
	:ComponentUI(COMPONENT_TYPE::RIGIDBODY2D)
{
}

RigidBody2DUI::~RigidBody2DUI()
{
}

void RigidBody2DUI::Render_UI()
{
	if (!IsValid(GetTarget()) || GetTarget()->RigidBody2D() == nullptr)
	{
		SetActive(false);
		return;
	}

	GRigidBody2D* pRigid = (GRigidBody2D*)GetTarget()->GetComponent(COMPONENT_TYPE::RIGIDBODY2D);

	OutputTitle("RigdBody2D");

	// Å©±â
	ImGui::Text("Velocity");
	ImGui::SameLine(GetTab());
	Vector2 vVelocity = pRigid->GetVelocity();
	ImGui::DragFloat2("##Velocity", vVelocity, 1.0f, 0.0f, 0.0f, nullptr, ImGuiSliderFlags_NoInput);
	AddItemHeight();

	ImGui::Text("Mass");
	ImGui::SameLine(GetTab());
	float fMass = pRigid->GetMass();
	if (ImGui::DragFloat("##Mass", &fMass))
	{
		pRigid->SetMass(fMass);
	}
	AddItemHeight();

	ImGui::Text("Gravity");
	ImGui::SameLine(GetTab());
	float fGravity = pRigid->GetGravity();
	if (ImGui::DragFloat("##Gravity", &fGravity))
	{
		pRigid->SetGravity(fGravity);
	}
	AddItemHeight();

	ImGui::Text("TerminalSpeed");
	ImGui::SameLine(GetTab());
	float fTerminalSpeed = pRigid->GetTerminalSpeed();
	if (ImGui::DragFloat("##TerminalSpeed", &fTerminalSpeed))
	{
		pRigid->SetTerminalSpeed(fTerminalSpeed);
	}
	AddItemHeight();

	ImGui::Text("Friction");
	ImGui::SameLine(GetTab());
	float fFriction = pRigid->GetFriction();
	if (ImGui::DragFloat("##Friction", &fFriction))
	{
		pRigid->SetFriction(fFriction);
	}
	AddItemHeight();

}


