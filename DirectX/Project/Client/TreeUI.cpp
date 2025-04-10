#include "pch.h"
#include "TreeUI.h"

#pragma region TreeNode

UINT TreeNode::g_NextID = 0;

TreeNode::TreeNode()
	: TreeUI(nullptr)
	, ID(g_NextID++)
	, Name("")
	, Data()
	, ParentNode(nullptr)
	, vecChildNode{}
	, Frame(false)
{

}

TreeNode::~TreeNode()
{
	Delete_Vec(vecChildNode);
}

void TreeNode::Render_UI()
{
	int flag = 0;
	string name = Name; 

	// �ڽ� ��尡 ���ٸ� ȭ��ǥ ǥ�� ����
	if (vecChildNode.empty())
	{
		flag |= ImGuiTreeNodeFlags_Leaf;
	}

	// Frame�� True��� Frame ǥ��
	if (Frame)
	{
		flag |= ImGuiTreeNodeFlags_Framed;
		if (vecChildNode.empty())
			name = "   " + name;
	}

	// ���� ��尡 ���õ� ����� ���� ǥ��
	if (TreeUI->GetSelectedNode() == this)
	{
		flag |= ImGuiTreeNodeFlags_Selected;
	}

	// ���ϸ� ���̰� �� ���, ��ι��ڿ����� �̸��� �����ؼ� ���̰� ��
	if (TreeUI->IsShowOnlyName())
	{
		name = path(Name).stem().string();
	}

	// ��� �̸� �ڿ� ���� ID ���̱� (Key�� Ȥ�ó� ���� �� �ߺ� ����)
	char buffer[255];
	sprintf_s(buffer, 255, "%s##%d", name.c_str(), ID);
	//name = name + "##" + to_string(ID);
	bool bSelected = false;

	if (ImGui::TreeNodeEx(buffer, flag))
	{
		// ��尡 Ŭ���Ǿ����� Ȯ��
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None)) bSelected = true;

		SelfDragCheck();
		SelftDropCheck();
		DropCheck();

		for (size_t i = 0;i < vecChildNode.size();++i)
		{
			vecChildNode[i]->Render_UI();
		}
		ImGui::TreePop();
	}
	else
	{
		// ��尡 Ŭ���Ǿ����� Ȯ��
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None)) bSelected = true;

		SelfDragCheck();
		SelftDropCheck();
		DropCheck();
	}

	// �̳�尡 ���õǾ��ٸ� UI���� �� ��尡 ���õǾ����� �˷���
	if (bSelected)
		TreeUI->SetSelected(this);
}

void TreeNode::SelfDragCheck()
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		// payload ���
		// Node�� ������ ��ü(TreeUI)�� �̸��� Payload Ű������ ����Ѵ�.
		// ������ �����ʹ� Node�� ��� �ִ� Data�̴�.
		ImGui::SetDragDropPayload(TreeUI->GetName().c_str(), &Data, sizeof(DWORD_PTR));

		// ���콺�� ����ٴϸ鼭 � �׸��� �巡���Ѱ��� ������ �����ִ� �뵵
		ImGui::Text(Name.c_str());

		// Tree���� �ڽ��� Drag ���� ����� ���� �˸�
		TreeUI->SetDragNode(this);

		ImGui::EndDragDropSource();
	}
}

void TreeNode::SelftDropCheck()
{

	if (!TreeUI->IsSelfDragDrop())
		return;

	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI���� �巡�׵� �����͸� �޴´�.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(TreeUI->GetName().c_str());

		// Tree���� �ڽ��� Drop ���� ����� ���� �˸�
		if (nullptr != payload)
			TreeUI->SetDropNode(this);

		ImGui::EndDragDropTarget();
	}
}

void TreeNode::DropCheck()
{
	if (!m_DragDrop)
		return;

	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI���� �巡�׵� �����͸� �޴´�.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_DUI.c_str());

		// Tree���� �ڽ��� Drop ���� ����� ���� �˸�
		if (nullptr != payload)
			(m_DDInst->*m_DDFunc)((DWORD_PTR)payload, (DWORD_PTR)this);

		ImGui::EndDragDropTarget();
	}
}

#pragma endregion




TreeUI::TreeUI()
	: m_Root(nullptr)
	, m_ShowRoot(false)
	, m_ShowOnlyName(true)
	, m_SelectedNode(nullptr)
	, m_SelectedInst(nullptr)
	, m_SelectedFunc()
	, m_SelectedChange(false)
	, m_DragNode(nullptr)
	, m_DropNode(nullptr)
	, m_SDDInst(nullptr)
	, m_SDDFunc(nullptr)
	, m_SelfDragDrop(false)
{
	SetName("TreeUI");

	m_Root = new TreeNode;
	m_Root->Name = "Root";
	m_Root->TreeUI = this;
}

TreeUI::~TreeUI()
{
	if (nullptr != m_Root)
		delete m_Root;
}

void TreeUI::Clear()
{
	if (nullptr != m_Root)
	{
		Delete_Vec(m_Root->vecChildNode);
	}
}

void TreeUI::Init()
{

}

void TreeUI::Update_UI()
{

}

void TreeUI::Render_UI()
{
	// ��Ʈ��尡 ���ٸ� ����
	if (m_Root == nullptr)
		return;
	
	if(m_ShowRoot)
		m_Root->Render_UI();
	else
	{
		for (size_t i = 0;i < m_Root->vecChildNode.size();++i)
		{
			m_Root->vecChildNode[i]->Render_UI();
		}
	}

	// ���õ� ��尡 ������ �߻��ߴٸ� Ư�� �Լ� ȣ��
	if (m_SelectedChange)
	{
		m_SelectedChange = false;
		if (m_SelectedInst && m_SelectedFunc)
		{
			(m_SelectedInst->*m_SelectedFunc)((DWORD_PTR)m_SelectedNode);
		}
	}

	if ( m_SelfDragDrop && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (m_DragNode)
		{
			if (m_DragNode != m_DropNode && (m_SDDInst && m_SDDFunc))
			{
				(m_SDDInst->*m_SDDFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
			}
		}

		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(TreeNode* _ParentNode, const string& _Name, DWORD_PTR _Data)
{
	TreeNode* pNode = new TreeNode;
	pNode->ParentNode = _ParentNode;
	pNode->Name = _Name;
	pNode->Data = _Data;
	pNode->TreeUI = _ParentNode->TreeUI;

	_ParentNode->vecChildNode.push_back(pNode);

	return pNode;
}

void TreeUI::SetSelected(TreeNode* _TreeNode)
{
	// ���� ������ ���� ������ ������ ��尡 ���ٸ� �ٷ� ����
	if (m_SelectedNode == _TreeNode)
		return;
	m_SelectedChange = true;
	m_SelectedNode = _TreeNode;
}




