#pragma once

#include "imgui/imgui.h"

class EditorUI
{
private:
	static UINT			UINextID;
	const UINT			m_EditorUI_ID;

	string				m_Name;
	bool				m_Active;

	EditorUI*			m_ParentUI;
	vector<EditorUI*>	m_vecChildUI;

	ImVec2				m_ChildSize;

	bool				m_Modal;
	bool				m_Seperator;	// 구분선 유무
	bool				m_Focused;		// 자신이 포커싱 되어있는 상태인지 확인
	bool				m_HorizontalScrollbar;

	ImGuiWindowFlags					m_WindowFlag;

	float				m_Tab;

public:
	UINT GetID() { return m_EditorUI_ID; }

	void SetName(const string& _Name) { m_Name = _Name; }
	const string& GetName() { return m_Name; }

	bool IsActive() { return m_Active; }
	void SetActive(bool _Active);

	void SetModal(bool _Modal) { m_Modal = _Modal; }
	void SetHorizontalScrollbar(bool _HorizontalScrollbar) { 
		m_HorizontalScrollbar = _HorizontalScrollbar; 
		m_WindowFlag ^= ImGuiWindowFlags_HorizontalScrollbar;
	}

	void AddChildUI(EditorUI* _ChildUI)
	{
		m_vecChildUI.push_back(_ChildUI);
		_ChildUI->m_ParentUI = this;
	}
	void RemoveChildUI(EditorUI* _ChildUI)
	{
		vector<EditorUI*>::iterator iter = find(m_vecChildUI.begin(), m_vecChildUI.end(), _ChildUI);
		assert(iter != m_vecChildUI.end());

		m_vecChildUI.erase(iter);
		_ChildUI->m_ParentUI = nullptr;
	}
	const vector<EditorUI*>& GetChildsUI()
	{
		return m_vecChildUI;
	}

	void SetSizeAsChild(ImVec2 _Size) { m_ChildSize = _Size; }
	ImVec2 GetSizeAsChild() { return m_ChildSize; }

	// 구분선 유무
	void SetSeperator(bool _Seperator) { m_Seperator; }	

	float GetTab() { return m_Tab; }

public:
	virtual void Init() {};	// UI 초기화
	virtual void Update();	// 매 프레임마다 UI 가 할 일
	virtual void Render();	// UI 렌ㄷ링

	virtual void Activate() {};
	virtual void Deactivate() {};

	virtual void Render_UI() = 0;
	virtual void Update_UI() = 0;

	virtual void OnFocus() {};
	virtual void OffFocus() {};

private:
	void CheckFocus();

public:
	EditorUI();
	virtual ~EditorUI();
};

