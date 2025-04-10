#include "pch.h"
#include "RenderComponentUI.h"

#include <Engine/GAssetManager.h>
#include "ImGuiManager.h"
#include "ListUI.h"

#include <Engine/GRenderComponent.h>
#include <Engine/GMesh.h>
#include <Engine/GMaterial.h>

RenderComponentUI::RenderComponentUI(COMPONENT_TYPE _Type)
	:ComponentUI(_Type)
{

}

RenderComponentUI::~RenderComponentUI()
{
}

void RenderComponentUI::SelectMesh(DWORD_PTR _UI)
{
	ListUI* pList = (ListUI*)_UI;
	Ptr<GMesh> pMesh = GAssetManager::GetInst()->FindAsset<GMesh>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));
	GRenderComponent* pRender = GetTarget()->Renderer();

	pRender->SetMesh(pMesh);
}

void RenderComponentUI::SelectMaterial(DWORD_PTR _UI)
{
	ListUI* pList = (ListUI*)_UI;
	Ptr<GMaterial> pMaterial = GAssetManager::GetInst()->FindAsset<GMaterial>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));
	GRenderComponent* pRender = GetTarget()->Renderer();

	pRender->SetMaterial(pMaterial);
}

void RenderComponentUI::Render_UI()
{
	MeshUI();
	MateiralUI();
}

void RenderComponentUI::MeshUI()
{
	GRenderComponent* pRender = GetTarget()->Renderer();

	// Mesh
	Ptr<GMesh> pMesh = pRender->GetMesh();
	string Key = "";
	if (pMesh != nullptr)
		Key = string().assign(pMesh->GetKey().begin(), pMesh->GetKey().end());
	ImGui::Text("Mesh");
	ImGui::SameLine(GetTab());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##MeshName", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// Drop Check
	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI에서 드르개된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// 전달된 데이터에 들어있는 Asset의 주소값을 꺼내온다.
			GAsset* pAsset = *((GAsset**)payload->Data);

			// 꺼내온 에셋이 Mesh 타입인 경우에만 작업을 이어간다.
			if (ASSET_TYPE::MESH == pAsset->GetType())
			{
				GMesh* pMesh = dynamic_cast<GMesh*>(pAsset);
				assert(pMesh);
				pRender->SetMesh(pMesh);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("...##Mesh", ImVec2(28.f, 0.f)))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::MESH, Items);
		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&RenderComponentUI::SelectMesh);
	}
	AddItemHeight();

}

void RenderComponentUI::MateiralUI()
{
	GRenderComponent* pRender = GetTarget()->Renderer();

	// Material
	Ptr<GMaterial> pMat = pRender->GetMaterial();
	string Key = "";
	if (pMat != nullptr)
		Key = string().assign(pMat->GetKey().begin(), pMat->GetKey().end());
	ImGui::Text("Material");
	ImGui::SameLine(GetTab());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##MaterialName", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// Drop Check
	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI에서 드르개된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// 전달된 데이터에 들어있는 Asset의 주소값을 꺼내온다.
			GAsset* pAsset = *((GAsset**)payload->Data);

			// 꺼내온 에셋이 Mesh 타입인 경우에만 작업을 이어간다.
			if (ASSET_TYPE::MATERIAL == pAsset->GetType())
			{
				GMaterial* pMaterial = dynamic_cast<GMaterial*>(pAsset);
				assert(pMaterial);
				pRender->SetMaterial(pMaterial);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("...##Material", ImVec2(28.f, 0.f)))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::MATERIAL, Items);
		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&RenderComponentUI::SelectMaterial);
	}
	AddItemHeight();
}
