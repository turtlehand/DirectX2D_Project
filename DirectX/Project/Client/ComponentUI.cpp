#include "pch.h"
#include "ComponentUI.h"
#include <Engine/GGameObject.h>
#include <Engine/GComponent.h>

#include "ImGuiManager.h"
#include "InspectorUI.h"

ComponentUI::ComponentUI(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Target(nullptr)
	, m_ItemHeight(0.f)
{
	SetName(COMPONENT_TYPE_STR[(int)m_Type]);
	SetSeperator(true);
	SetHorizontalScrollbar(true);
	SetActive(false);
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::OutputTitle(const string& _Name)
{
	m_ItemHeight = 0.f;

	ImGui::Separator();
	AddItemHeight();

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f,0.6f,0.3f,1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.6f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.6f, 0.3f, 1.f));

	if (_Name == "")
		ImGui::Button(GetName().c_str());
	else
		ImGui::Button(_Name.c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();


	// 삭제 버튼
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{
		ImGui::OpenPopup("Delete Component");
	}

	if (ImGui::BeginPopup("Delete Component"))
	{
		ImGui::Text("Delete this Component.");
		ImGui::Text("Are You Sure?");
		if (ImGui::Button("OK"))
		{
			DeleteComponent();
			SetActive(false);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("NO"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}


	AddItemHeight();
}

void ComponentUI::SetTarget(GGameObject* _Target)
{
	m_Target = _Target;
	// m_Target이 nullptr이거나, ComponentUI가 담당하는 Component를 GameObjct가 가지고 있지 않는다면
	if (nullptr == m_Target || nullptr == m_Target->GetComponent(m_Type))
		SetActive(false);
	else
		SetActive(true);
}

void ComponentUI::Update_UI()
{
	SetSizeAsChild(ImVec2(0.f, m_ItemHeight));
}

void ComponentUI::AddItemHeight()
{
	ImVec2 vSize = ImGui::GetItemRectSize();
	m_ItemHeight += vSize.y + 5.f;
}

void ComponentUI::DeleteComponent()
{
	m_Target->GetComponent(m_Type)->Destroy();
}
