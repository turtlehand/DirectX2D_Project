#include "pch.h"
#include "ListUI.h"
#include <Engine/GKeyManager.h>

ListUI::ListUI()
	:m_Idx(-1)
{
}

ListUI::~ListUI()
{
}

void ListUI::SetItems(const vector<string>& _Items)
{
	m_vecStr = _Items;
}

void ListUI::AddDelegate(EditorUI* _TargetUI, EUI_DELEGATE_1 _MemFun1)
{
	m_TargetUI = _TargetUI;
	m_MemFun1 = _MemFun1;
}

void ListUI::Init()
{
	m_Idx = -1;
	m_vecStr.clear();
	m_Selected_string = "";
	m_TargetUI = nullptr;
	m_MemFun1 = nullptr;
}

void ListUI::Update_UI()
{
	if (KEY_DOWN(KEY::ESC))
		SetActive(false);
}

void ListUI::Render_UI()
{
	for (int i = 0; i < m_vecStr.size(); i++)
	{
		if (ImGui::Selectable(m_vecStr[i].c_str(), m_Idx == i, ImGuiSelectableFlags_AllowDoubleClick))
		{ 
			m_Idx = i;
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_Selected_string = m_vecStr[m_Idx];
				m_Selected_int = m_Idx;
				SetActive(false);

				if (m_TargetUI && m_MemFun1)
					(m_TargetUI->*m_MemFun1)((DWORD_PTR)this);
			}
		}
	}

}

void ListUI::Activate()
{
	Init();
}

void ListUI::Deactivate()
{
	ImGui::SetWindowFocus(nullptr);
}


