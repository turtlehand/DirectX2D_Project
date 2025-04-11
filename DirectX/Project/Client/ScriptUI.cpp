#include "pch.h"
#include "ScriptUI.h"

#include <Engine/GScript.h>
#include <Engine/GAsset.h>
#include <Engine/GPrefab.h>

#include <Practice/GScriptManager.h>

ScriptUI::ScriptUI()
	: ComponentUI(COMPONENT_TYPE::SCRIPT)
	, m_TargetScript(nullptr)
{
	// ScriptUI 마다 이름이 다르게 설정되도록 함
	char szBuff[10] = {};
	_itoa_s(GetID(), szBuff, 10);

	// 기존 이름에 뒤에 안겹치는 숫자를 붙여줌
	SetName(GetName() + szBuff);
	string Name = GetName();
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::SetScript(GScript* _Script)
{
	m_TargetScript = _Script;
	if (nullptr == m_TargetScript)
	{
		SetActive(false);
	}
	else
		SetActive(true);
}

void ScriptUI::DeleteComponent()
{
	m_TargetScript->Destroy();
}

void ScriptUI::Render_UI()
{
	if (nullptr == GScriptManager::GetScriptName(m_TargetScript))
	{
		SetActive(false);
		return;
	}

	wstring wScriptName = GScriptManager::GetScriptName(m_TargetScript);
	string ScriptName = string(wScriptName.begin(), wScriptName.end());
	OutputTitle(ScriptName);

	const vector<tScriptParam>& ScriptParam = m_TargetScript->GetScriptParam();
	for (size_t i = 0; i < ScriptParam.size(); ++i)
	{
		char szID[50] = {};
		sprintf_s(szID, 50, "##WidgetID_%d", (int)i);

		switch (ScriptParam[i].Type)
		{
		case SCRIPT_PARAM::BOOL:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(GetTab());
			ImGui::Checkbox(szID, (bool*)ScriptParam[i].pData);
			AddItemHeight();
		}
		break;
		case SCRIPT_PARAM::INT:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(GetTab());
			ImGui::InputInt(szID, (int*)ScriptParam[i].pData);
			AddItemHeight();
		}
		break;
		case SCRIPT_PARAM::FLOAT:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(GetTab());
			ImGui::InputFloat(szID, (float*)ScriptParam[i].pData);
			AddItemHeight();
		}
		break;
		case SCRIPT_PARAM::VEC2:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(GetTab());
			ImGui::InputFloat2(szID, *((Vector2*)ScriptParam[i].pData));
			AddItemHeight();
		}
		break;
		case SCRIPT_PARAM::VEC3:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(GetTab());
			ImGui::InputFloat3(szID, *((Vector3*)ScriptParam[i].pData));
			AddItemHeight();
		}
		break;
		case SCRIPT_PARAM::VEC4:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(GetTab());
			ImGui::InputFloat4(szID, *((Vector4*)ScriptParam[i].pData));
			AddItemHeight();
		}
		break;
		case SCRIPT_PARAM::TEXTURE:
		{
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::PREFAB:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(GetTab());
			Ptr<GPrefab> pPrefab = *((Ptr<GPrefab>*)ScriptParam[i].pData);

			string AssetName;
			if (nullptr != pPrefab)
				AssetName = ToString(pPrefab->GetKey());

			ImGui::InputText(szID, (char*)AssetName.c_str(), AssetName.length() + 1, ImGuiInputTextFlags_ReadOnly);

			// Drop Check
			if (ImGui::BeginDragDropTarget())
			{
				// ContentUI 에서 드래그된 데이터만 받는다.
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

				if (nullptr != payload)
				{
					// 전달된 데이터에 들어있는 Asset 의 주소값을 꺼내온다.
					GAsset* pAsset = *((GAsset**)payload->Data);

					// 꺼내온 에셋이 Mesh 타입인 경우에만 작업을 이어간다.
					if (ASSET_TYPE::PREFAB == pAsset->GetType())
					{
						pPrefab = dynamic_cast<GPrefab*>(pAsset);
						assert(pPrefab.Get());
					}
				}

				ImGui::EndDragDropTarget();
			}

			// 목적지에 넣어준다.
			*((Ptr<GPrefab>*)ScriptParam[i].pData) = pPrefab;

			AddItemHeight();
		}
		break;
		}
	}

}


