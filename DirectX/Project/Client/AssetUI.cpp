#include "pch.h"
#include "AssetUI.h"

#include <Engine/GPathManager.h>

AssetUI::AssetUI(ASSET_TYPE _Type)
	:m_AssetType(_Type)
{
	SetName(ASSET_TYPE_STR[(int)m_AssetType]);
	SetSeperator(true);
	SetActive(false);
}

AssetUI::~AssetUI()
{
}

void AssetUI::OutputTitle(const string& _Name)
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.8f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.8f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.8f, 1.f));
	ImGui::Button(_Name.c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		wstring Path =  GPathManager::GetContentPath() + m_Asset->GetRelativePath();
		m_Asset->Save(Path);
	}

	ImGui::Spacing();
	ImGui::Spacing();


}

void AssetUI::OutputKeyPath()
{
	string Key = string(m_Asset->GetKey().begin(), m_Asset->GetKey().end());
	string Path = string(m_Asset->GetRelativePath().begin(), m_Asset->GetRelativePath().end());

	ImGui::Text("Key");
	ImGui::SameLine(GetTab());
	ImGui::InputText("##AssetKey", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Path");
	ImGui::SameLine(GetTab());
	ImGui::InputText("##AssetPath", (char*)Path.c_str(), Path.length() + 1, ImGuiInputTextFlags_ReadOnly);
}

void AssetUI::SetAsset(Ptr<GAsset> _Asset)
{
	// 담당하는 Asset과 다른 타입의 Asset의 입력으로 들어온 경우
	if (nullptr == _Asset || m_AssetType != _Asset->GetType())
		SetActive(false);
	else
	{ 
		SetActive(false);
		SetActive(true);
		m_Asset = _Asset;
	}

}