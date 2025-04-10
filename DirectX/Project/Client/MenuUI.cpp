#include "pch.h"
#include "MenuUI.h"

#include "InspectorUI.h"
#include "SpriteEditorUI.h"
#include "ImGuiManager.h"
#include "GLevelSaveLoad.h"

#include "EditorManager.h"

#include "Engine/GGameObject.h"
#include <Engine/GAssetManager.h>
#include <Engine/GMaterial.h>

#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include <Engine/GTaskManager.h>

#include <Engine/GComponent.h>
#include <Engine/GScript.h>

#include <Practice/GScriptManager.h>


#include <commdlg.h>

MenuUI::MenuUI()
	: m_FlipbookEditor(false)
	, m_CloneLevel(nullptr)
{
}

MenuUI::~MenuUI()
{
	delete m_CloneLevel;
}

void MenuUI::Render()
{
	if (!IsActive())
		return;
	if (ImGui::BeginMainMenuBar())
	{
		Render_UI();
		ImGui::EndMainMenuBar();
	}
}

void MenuUI::Update_UI()
{
}

void MenuUI::Render_UI()
{
	File();
	Level();
	Editor();
	Script();
	Asset();
}

void MenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		bool IsLevelStop = GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP;

		if (ImGui::MenuItem("Level Save",nullptr, nullptr, IsLevelStop))
		{
			wstring strContentPath = GPathManager::GetContentPath();
			strContentPath += L"Level";

			// ���� ��� ���ڿ�
			wchar_t szFilePath[255] = {};

			OPENFILENAME Desc = {};

			Desc.lStructSize = sizeof(OPENFILENAME);
			Desc.hwndOwner = nullptr;
			Desc.lpstrFile = szFilePath;	// ���������� �� ��θ� �޾Ƴ� ������
			Desc.nMaxFile = 255;
			Desc.lpstrFilter = L"Level\0*.lv\0ALL\0*.*";
			Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			Desc.lpstrInitialDir = strContentPath.c_str();

			if (GetSaveFileName(&Desc))
			{
				GLevelSaveLoad::SaveLevel(szFilePath);
			}

			// imgui.ini ���� ��� �ʱ�ȭ
			wstring strBinPath = GPathManager::GetBinPath();
			SetCurrentDirectory(strBinPath.c_str());
		}

		if (ImGui::MenuItem("Level Load", nullptr, nullptr, IsLevelStop))
		{
			wstring strContentPath = GPathManager::GetContentPath();
			strContentPath += L"Level";

			// ���� ��� ���ڿ�
			wchar_t szFilePath[255] = {};

			OPENFILENAME Desc = {};

			Desc.lStructSize = sizeof(OPENFILENAME);
			Desc.hwndOwner = nullptr;
			Desc.lpstrFile = szFilePath;
			Desc.nMaxFile = 255;
			Desc.lpstrFilter = L"Level\0*.lv\0ALL\0*.*";
			Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			Desc.lpstrInitialDir = strContentPath.c_str();

			if (GetOpenFileName(&Desc))
			{
				
				GLevel* pNextLevel = GLevelSaveLoad::LoadLevel(szFilePath);
				
				// ���� ����
				tTask task = {};
				task.Type = TASK_TYPE::CHANGE_LEVEL;
				task.Param0 = (DWORD_PTR)pNextLevel;

				GTaskManager::GetInst()->AddTask(task);

			}

			// imgui.ini ���� ��� �ʱ�ȭ
			wstring strBinPath = GPathManager::GetBinPath();
			SetCurrentDirectory(strBinPath.c_str());
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Setting()
{
	if (ImGui::BeginMenu("Setting"))
	{
		if (ImGui::MenuItem("Layer Name"))
		{

		}
		
		if (ImGui::MenuItem("Collision Check"))
		{

		}
	}
}

void MenuUI::Level()
{
	if (ImGui::BeginMenu("Level"))
	{

		GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();
		LEVEL_STATE State = pCurLevel->GetState();

		bool IsPlay = State == LEVEL_STATE::PLAY;
		bool IsPause = State == LEVEL_STATE::PAUSE;
		bool IsStop = State == LEVEL_STATE::STOP;

		tTask task = {};
		task.Type = TASK_TYPE::CHANGE_LEVEL_STATE;

		if (ImGui::MenuItem("Play", nullptr, &IsPlay, IsPause || IsStop))
		{
			// ���� ������ �����ص�
			GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();
			m_CloneLevel = pCurLevel->Clone();

			task.Param0 = (DWORD_PTR)LEVEL_STATE::PLAY;
			GTaskManager::GetInst()->AddTask(task);
		}

		if (ImGui::MenuItem("Pause", nullptr, &IsPause, IsPlay))
		{
			task.Param0 = (DWORD_PTR)LEVEL_STATE::PAUSE;
			GTaskManager::GetInst()->AddTask(task);
		}

		if (ImGui::MenuItem("Stop", nullptr, &IsStop, IsPlay || IsPause))
		{
			// ������ ������ Stop ���·� �����.
			task.Param0 = (DWORD_PTR)LEVEL_STATE::STOP;
			GTaskManager::GetInst()->AddTask(task);

			// Play ��ư�� ������ ��� ���¸� �����س��� ������ �ٽ� �����Ѵ�.
			tTask LevelChangeTask = {};
			LevelChangeTask.Type = TASK_TYPE::CHANGE_LEVEL;
			LevelChangeTask.Param0 = (DWORD_PTR)m_CloneLevel;
			GTaskManager::GetInst()->AddTask(LevelChangeTask);
			m_CloneLevel = nullptr;		// �����ߴ� ������ ���� ������ش�.

			// Inspector�� �����ִ� ������Ʈ�� ������.
			InspectorUI* pInspector = ImGuiManager::GetInst()->FindUI<InspectorUI>("InspectorUI");
			pInspector->SetTargetObject(nullptr);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Editor()
{
	if (ImGui::BeginMenu("Editor"))
	{

		if (ImGui::MenuItem("Sprite Editor", nullptr, nullptr))
		{
			ImGuiManager::GetInst()->FindUI<SpriteEditorUI>("SpriteEditorUI")->SetActive(true);
		}

		if (ImGui::MenuItem("Camera"))
		{
			ImGuiManager::GetInst()->FindUI<InspectorUI>("InspectorUI")->SetTargetObject((GGameObject*)(EditorManager::GetInst()->GetEditorCamera()));
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Script()
{
	if (ImGui::BeginMenu("Scripts"))
	{
		vector<wstring> vecScriptName;
		GScriptManager::GetScriptInfo(vecScriptName);

		for (int i = 0; i < vecScriptName.size(); ++i)
		{
			if (ImGui::MenuItem(string(vecScriptName[i].begin(),vecScriptName[i].end()).c_str()))
			{
				InspectorUI* pInspector = ImGuiManager::GetInst()->FindUI<InspectorUI>("InspectorUI");

				GGameObject* pObject = pInspector->GetTargetObject();

				if (nullptr != pObject)
				{
					pObject->AddComponent(GScriptManager::GetScript(vecScriptName[i]));
					pInspector->SetTargetObject(pObject);
				}
			}
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Asset()
{
	if (ImGui::BeginMenu("Asset"))
	{
		if (ImGui::MenuItem("Create Material"))
		{
			CreateDefaultAsset<GMaterial>(L"Material");
		}

		if (ImGui::MenuItem("Create FlipBook"))
		{
			CreateDefaultAsset<GFlipbook>(L"Flipbook");
		}

		if (ImGui::MenuItem("Create TilePalette"))
		{
			CreateDefaultAsset<GTilePalette>(L"TilePalette");
		}

		ImGui::EndMenu();
	}
}





