#include "pch.h"
#include "CameraUI.h"

#include <Engine/GCamera.h>
#include <Engine/GLayer.h>
#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>

CameraUI::CameraUI()
	:ComponentUI(COMPONENT_TYPE::CAMERA)
{

}

CameraUI::~CameraUI()
{
}

void CameraUI::Render_UI()
{
	if (!IsValid(GetTarget()))
		return;

	GCamera* pCamera = (GCamera*)GetTarget()->GetComponent(COMPONENT_TYPE::CAMERA);
	
	OutputTitle("Camera");

	ImGui::Text("Projection");
	ImGui::SameLine(GetTab());
	const char* items[] = { "ORTHOGRAPHIC", "PERSPECTIVE" };
	int type = (int)pCamera->GetProjType();
	ImGui::Combo("##combo", &type, items, IM_ARRAYSIZE(items));
	if (type != (int)pCamera->GetProjType())
		pCamera->SetProjType((PROJECTION_TYPE)type);
	AddItemHeight();

	// 카메라 우선순위
	ImGui::BeginDisabled(GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::STOP);
	{
		int Priority = pCamera->GetPriority();
		ImGui::Text("Priority");
		ImGui::SameLine(GetTab());
		if (ImGui::DragInt("##Priority", &Priority, 1, - 1, 32))
		{
			pCamera->SetPriority(Priority);
		}
	}
	ImGui::EndDisabled();
	AddItemHeight();

	// Width
	ImGui::BeginDisabled(PROJECTION_TYPE::ORTHOGRAPHIC != pCamera->GetProjType());
	{
		float OrthoScale = pCamera->GetOrthoScaleX();
		ImGui::Text("Width");
		
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##OrthoScale", &OrthoScale))
		{
			pCamera->SetOrthoScaleX(OrthoScale);
		}
	}
	ImGui::EndDisabled();
	AddItemHeight();

	// FOV
	ImGui::BeginDisabled(PROJECTION_TYPE::PERSPECTIVE != pCamera->GetProjType());
	{
		float FOV = pCamera->GetFOV();
		ImGui::Text("FOV");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##FOV", &FOV, 1.f, 1.f, 180.f))
		{
			pCamera->SetFOV(FOV);
		}
	}
	ImGui::EndDisabled();
	AddItemHeight();

	// 가로 세로 종횡비
	float AspectRatio = pCamera->GetAspectRatio();
	ImGui::Text("AspectRatio");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat("##AspectRatio", &AspectRatio, 0.01f))
	{
		pCamera->SetAspectRatio(AspectRatio);
	}
	AddItemHeight();

	// 최대 시야 거리
	float Far = pCamera->GetFar();
	ImGui::Text("Far");
	ImGui::SameLine(GetTab());
	float FarMin = 100.f;
	if (ImGui::DragFloat("##Far", &Far, 10.f, 10.f, 10000.f))
	{
		pCamera->SetFar(Far);
	}
	AddItemHeight();
	
	
	// 카메라 렌더링 레이어
	ImGui::Text("LayerCheck");
	ImGui::SameLine(GetTab());
	if (ImGui::Button("...##LayerCheck"))
		ImGui::OpenPopup("LayerCheck");
	if (ImGui::BeginPopup("LayerCheck"))
	{
		bool layer[MAX_LAYER];
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			layer[i] = pCamera->GetLayerCheck(i);
			wstring LayerName = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(i)->GetName();
			string LayerText = to_string(i) + ". ";
			LayerText += string().assign(LayerName.begin(), LayerName.end());
			if (ImGui::Selectable(LayerText.c_str(), &layer[i], ImGuiSelectableFlags_DontClosePopups))
			{
				pCamera->CheckLayer(i);
			}
		}
		ImGui::EndPopup();
	}
	AddItemHeight();

}


