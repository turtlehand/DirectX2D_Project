#include "pch.h"
#include "InspectorUI.h"

#include <Engine/GAssetManager.h>

#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>
#include <Engine/GGameObject.h>
#include <Engine/components.h>

#include "ComponentUI.h"
#include "AddComponentUI.h"

#include "ui_component.h"
#include "ui_asset.h"

#include "ImGuiManager.h"
#include "OutlinerUI.h"

InspectorUI::InspectorUI()
	: m_Object(nullptr)
	, m_AssetUI{}
	, m_ComUI{}
{

}

InspectorUI::~InspectorUI()
{

}

void InspectorUI::Init()
{
	CreateComponentUI();
	CreateAssetUI();
}

void InspectorUI::Update_UI()
{
	/*
	if (nullptr == m_Object)
	{
		GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();
		SetTarget(pCurLevel->FindObjectByName(L"Camera"));
	}
	*/
}

void InspectorUI::Render_UI()
{
	if (IsValid(m_Object))
	{
		// =======
		// Name
		ImGui::Text("Name");

		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		string name = string(m_Object->GetName().begin(), m_Object->GetName().end());
		char buffer[255];
		strcpy_s(buffer, name.c_str());
		if (ImGui::InputText("##ObjectName", buffer, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			wstring name = wstring(buffer, buffer + 255);
			m_Object->SetName(name);
			OutlinerUI* pOutLiner = ImGuiManager::GetInst()->FindUI<OutlinerUI>("OutlinerUI");
			pOutLiner->Renew();
		}

		// =======
		// Layer
		ImGui::SameLine();
		ImGui::Text("Layer");

		ImGui::SameLine();
		ImGui::SetNextItemWidth(200);
		int CurLayer = m_Object->GetLayer();

		if(ImGui::Combo("##ObjectLayer", &CurLayer, GLevelManager::GetInst()->GetLayerNames(), MAX_LAYER))
		{
			ChangeGameObjectLayer(m_Object, CurLayer);
			OutlinerUI* pOutLiner = ImGuiManager::GetInst()->FindUI<OutlinerUI>("OutlinerUI");
			pOutLiner->Renew();
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			ImGui::OpenPopup("Delete Object");
		}

		if (ImGui::BeginPopup("Delete Object"))
		{
			ImGui::Text("Delete this Object.");
			ImGui::Text("Are You Sure?");
			if (ImGui::Button("OK"))
			{
				m_Object->Destroy();
				SetTargetObject(nullptr);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("NO"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SetNextItemWidth(200);

		ImGui::Separator();

	}
	else if (nullptr != m_Asset)
	{
		ImGui::Text("Key");
		ImGui::SameLine();
		string key = string(m_Asset->GetKey().begin(), m_Asset->GetKey().end());
		string name = path(key).stem().string();
		char buffer[255];
		strcpy_s(buffer, name.c_str());
		//ImGui::InputText("##AssetKey", buffer, 255, ImGuiInputTextFlags_ReadOnly);
		
		if (ImGui::InputText("##AssetKey", buffer, 255, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			wstring wname = wstring(buffer, buffer + 255);
			GAssetManager::GetInst()->ReSetKey(m_Asset->GetType(), m_Asset->GetKey(), wname);
		}
		
		ImGui::Separator();
	}
	
}

void InspectorUI::SetTargetObject(GGameObject* _Objcet)
{
	m_Object = _Objcet;
	m_Asset = nullptr;
	// Inspector의 자식UI(ComponentUI)들에게도 Object를 알려준다.
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_ComUI[i])
			continue;
		m_ComUI[i]->SetTarget(m_Object);
	}

	// Inspector가 보유한 ScriptUI 개수가 Object가 보유한 Script 개수보다 적으면, 추가로 더 생성한다.
	if (nullptr != m_Object)
	{
		size_t ScriptCount = m_Object->GetScripts().size();
		if (m_vecScriptUI.size() < ScriptCount)
		{
			// AddComponentUI를 맨끝으로 보낸다.
			RemoveChildUI(m_AddComponentUI);

			int SpawnCount = ScriptCount - m_vecScriptUI.size();
			for (int i = 0; i < SpawnCount; ++i)
			{
				ScriptUI* pScriptUI = new ScriptUI;
				AddChildUI(pScriptUI);
				m_vecScriptUI.push_back(pScriptUI);
			}
			
			AddChildUI(m_AddComponentUI);
		}
	}

	// Inspector의 모든 ScriptUI에게 Object 정보를 알려준다.
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		if (nullptr == m_Object || (m_Object->GetScripts().size() <= i))
			m_vecScriptUI[i]->SetScript(nullptr);
		else
			m_vecScriptUI[i]->SetScript(m_Object->GetScripts()[i]);
	}

	m_AddComponentUI->SetTarget(m_Object);

	// 에셋 UI들은 비활성화 시켜준다.
	for (UINT i = 0; i < (int)ASSET_TYPE::END; ++i)
	{
		if (nullptr == m_AssetUI[i])
			continue;
		m_AssetUI[i]->SetActive(false);
	}
}

void InspectorUI::SetAsset(Ptr<GAsset> _Asset)
{
	m_Asset = _Asset;
	m_Object = nullptr;
	for (UINT i = 0; i < (int)ASSET_TYPE::END; ++i)
	{
		if (nullptr == m_AssetUI[i])
			continue;
		m_AssetUI[i]->SetAsset(m_Asset);
	}

	// ComponentUI 들은 비활성화시켜준다.ㄴ
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_ComUI[i])
			continue;
		m_ComUI[i]->SetActive(false);
	}

	// 스크립트UI를 비활성화시켜준다.
	for (UINT i = 0; i < m_vecScriptUI.size(); ++i)
	{
		m_vecScriptUI[i]->SetActive(false);
	}

	m_AddComponentUI->SetActive(false);

}



