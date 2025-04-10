#include "pch.h"
#include "EditorUI.h"

UINT EditorUI::UINextID = 0;

EditorUI::EditorUI()
	: m_EditorUI_ID(UINextID++)
	, m_Active(true)
	, m_ParentUI(nullptr)
	, m_ChildSize(ImVec2(0.f, 0.f))
	, m_Modal(false)
	, m_Seperator(false)
	, m_Focused(false)
	, m_Tab(150.f)
	, m_WindowFlag(0)
	, m_HorizontalScrollbar(false)
{

}

EditorUI::~EditorUI()
{
	Delete_Vec(m_vecChildUI);
}

void EditorUI::Update()
{
	Update_UI();

	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void EditorUI::Render()
{
	if (!m_Active)
		return;

	// �ֻ��� �θ� UI �� ���
	if (nullptr == m_ParentUI)
	{
		// Modalless
		if (!m_Modal)
		{
			ImGui::Begin(m_Name.c_str(), &m_Active, m_WindowFlag);

			CheckFocus();

			Render_UI();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->Render();
			}

			ImGui::End();
		}

		// Modal
		else
		{
			ImGui::OpenPopup(m_Name.c_str());
			if (ImGui::BeginPopupModal(m_Name.c_str(), &m_Active, m_WindowFlag))
			{
				Render_UI();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->Render();
				}

				ImGui::EndPopup();
			}
		}
	}

	// �ڽ� UI  �� ���
	else
	{
		ImGui::BeginChild(m_Name.c_str(), m_ChildSize,0, m_WindowFlag);

		CheckFocus();

		Render_UI();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->Render();
		}

		ImGui::EndChild();

		// ���м�
		if(m_Seperator) ImGui::Separator();
	}
}

void EditorUI::SetActive(bool _Active)
{
	if (m_Active == _Active)
		return;

	m_Active = _Active;


	if (m_Active)
	{
		Activate();
	}
	else
		Deactivate();
}

void EditorUI::CheckFocus()
{
	// ���� ��Ŀ�� ���̶��
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
	{
		// �������� ��Ŀ���� �ƴϿ��ٸ�
		if (false == m_Focused)
		{
			OnFocus();
		}
		m_Focused = true;
	}
	else
	{
		// ���� �����ӿ��� ��Ŀ�� ���̾��ٸ�
		if (true == m_Focused)
		{
			OffFocus();
		}
		m_Focused = false;
	}
}