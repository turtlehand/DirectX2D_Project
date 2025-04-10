#pragma once
#include "EditorUI.h"
#include "ImGuiManager.h"

class TreeUI;

struct TreeNode
{
public:
	static UINT g_NextID;

private:
	TreeUI*             TreeUI;

	UINT                ID;
	string              Name;
	DWORD_PTR           Data;
	
	TreeNode*           ParentNode;
	vector<TreeNode*>   vecChildNode;

	bool                Frame;

	bool				m_DragDrop;
	string				m_DUI;		// Drag할 UI
	EditorUI*			m_DDInst;	// DragDrop 완료시 실행할 함수
	EUI_DELEGATE_2		m_DDFunc;

public:
	const string& GetName() { return Name; }

	void SetFrame(bool _Frame) { Frame = _Frame; }
	DWORD_PTR GetData() { return Data; }

	void AddDragDropDelegate(string _DragUI, EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
	{
		m_DUI = _DragUI;
		m_DDInst = _Inst;
		m_DDFunc = _MemFunc;
		m_DragDrop = true;
	}

public:
	void Render_UI();

private:
	void SelfDragCheck();
	void SelftDropCheck();

	void DropCheck();

public:
	TreeNode();
	~TreeNode();
	friend class TreeUI;

};

class TreeUI :
	public EditorUI
{
private:
	TreeNode*       m_Root;             // 보유하고 있는 노드의 루트

	bool            m_ShowRoot;         // Root의 이름을 보여줄지 여부(기본적으로 비활성화
	bool            m_ShowOnlyName;     // 경로를 제외한 파일명만 보여줄지 여부

	TreeNode*       m_SelectedNode;     // 현재 선택되고 있는 노드
	EditorUI*       m_SelectedInst;     // 해당 UI에게 선택되고 있는 노드의 Data를 넘겨준다.
	EUI_DELEGATE_1  m_SelectedFunc;     // 해당 UI가 노드의 Data를 받고 실행시킬 함수
	bool            m_SelectedChange;   // 노드 선택하였는지에 대한 bool

	TreeNode*       m_DragNode;			// 드래그 당한 노드    
	TreeNode*       m_DropNode;			// 드롭 당한 노드

	bool			m_SelfDragDrop;
	EditorUI*		m_SDDInst;
	EUI_DELEGATE_2	m_SDDFunc;

public:
	TreeNode* GetRoot() { return m_Root; }

	TreeNode* AddItem(TreeNode* _ParentNode,const string& _Name, DWORD_PTR _Data);      // 노드 추가
	void SetSelected(TreeNode* _TreeNode);												// 현재 노드가 선택됨
	TreeNode* GetSelectedNode() { return m_SelectedNode; }                              // 현재 선택되고 있는 노드 반환

	void SetDragNode(TreeNode* _Node) { m_DragNode = _Node; }
	void SetDropNode(TreeNode* _Node) { m_DropNode = _Node; }

	TreeNode* GetDragNode() { return m_DragNode; }
	TreeNode* GetDropNode() { return m_DropNode; }

	void AddSelectDelegate(EditorUI* _Inst, EUI_DELEGATE_1 _MemFunc)
	{
		m_SelectedInst = _Inst;
		m_SelectedFunc = _MemFunc;
	}

	void AddSelfDragDropDelegate(EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
	{
		m_SDDInst = _Inst;
		m_SDDFunc = _MemFunc;
		m_SelfDragDrop = true;
	}

	bool IsSelfDragDrop() { return m_SelfDragDrop; }

	void SetShowRoot(bool _Show) { m_ShowRoot = _Show; }

	void SetShowOnlyName(bool _Show) { m_ShowOnlyName = _Show; }
	bool IsShowOnlyName() { return m_ShowOnlyName; }

	void Clear();       // 루트 노드를 제외한 노드를 제거한다.

public:
	virtual void Init() override;
	virtual void Update_UI() override;
	virtual void Render_UI() override;

private:
	virtual void OffFocus() { m_SelectedNode = nullptr; }

public:
	TreeUI();
	~TreeUI();
};

