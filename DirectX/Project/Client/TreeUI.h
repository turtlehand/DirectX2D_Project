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
	string				m_DUI;		// Drag�� UI
	EditorUI*			m_DDInst;	// DragDrop �Ϸ�� ������ �Լ�
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
	TreeNode*       m_Root;             // �����ϰ� �ִ� ����� ��Ʈ

	bool            m_ShowRoot;         // Root�� �̸��� �������� ����(�⺻������ ��Ȱ��ȭ
	bool            m_ShowOnlyName;     // ��θ� ������ ���ϸ� �������� ����

	TreeNode*       m_SelectedNode;     // ���� ���õǰ� �ִ� ���
	EditorUI*       m_SelectedInst;     // �ش� UI���� ���õǰ� �ִ� ����� Data�� �Ѱ��ش�.
	EUI_DELEGATE_1  m_SelectedFunc;     // �ش� UI�� ����� Data�� �ް� �����ų �Լ�
	bool            m_SelectedChange;   // ��� �����Ͽ������� ���� bool

	TreeNode*       m_DragNode;			// �巡�� ���� ���    
	TreeNode*       m_DropNode;			// ��� ���� ���

	bool			m_SelfDragDrop;
	EditorUI*		m_SDDInst;
	EUI_DELEGATE_2	m_SDDFunc;

public:
	TreeNode* GetRoot() { return m_Root; }

	TreeNode* AddItem(TreeNode* _ParentNode,const string& _Name, DWORD_PTR _Data);      // ��� �߰�
	void SetSelected(TreeNode* _TreeNode);												// ���� ��尡 ���õ�
	TreeNode* GetSelectedNode() { return m_SelectedNode; }                              // ���� ���õǰ� �ִ� ��� ��ȯ

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

	void Clear();       // ��Ʈ ��带 ������ ��带 �����Ѵ�.

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

