#include "pch.h"
#include "ImGuiManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <Engine/GGameObject.h>
#include <Engine/GTransform.h>
#include <Engine/GRenderComponent.h>

#include "ui.h"

ImGuiManager::ImGuiManager()
{

}

ImGuiManager::~ImGuiManager()
{
	// CleanUp
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Delete_Map(m_mapUI);


	m_mapUI.clear();
}
void ImGuiManager::Progress()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Demo ��� �Ұ��� UI
	ImGui::ShowDemoWindow();

	for (const auto& pair : m_mapUI)
	{
		pair.second->Update();
	}

	for (const auto& pair : m_mapUI)
	{
		pair.second->Render();
	}


	// Rendering
	// ���� ������ ���ʿ� �ִ� UI ������
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// ���� ������ �ۿ� �ִ� ������ �����쿡 �׷����� UI ������
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiManager::CreateGameObject(GGameObject* _Parent)
{
	GGameObject* pObject = new GGameObject;
	pObject->SetName(L"Empty");

	if (IsValid(_Parent))
	{
		_Parent->AddChild(pObject);
	}
	pObject->AddComponent<GTransform>();
	SpawnGameObject(pObject);
}
