#include "pch.h"
#include "SpriteEditorUI.h"

#include <Engine/GAssetManager.h>
#include <Engine/GTexture.h>
#include <Engine/GSprite.h>

#include "MenuUI.h"

#include "ImGuiManager.h"
#include "ListUI.h"

SpriteEditorUI::SpriteEditorUI()
	: m_LeftTop()
	, m_Slice()
	, m_Gap()
	, m_RowCol()
	, m_Atlas(nullptr)
	, m_SpriteName()
{
}

SpriteEditorUI::~SpriteEditorUI()
{

}

void SpriteEditorUI::Init()
{
}

void SpriteEditorUI::Update_UI()
{
}

void SpriteEditorUI::Render_UI()
{
	InputSpriteValue();
	ImGui::SameLine();
	PreviewSprite();
}

void SpriteEditorUI::Activate()
{
	Reset();
}

void SpriteEditorUI::Deactivate()
{
}


void SpriteEditorUI::SelectTexture(DWORD_PTR _UI)
{
	ListUI* pListUI = (ListUI*)_UI;
	string KeyName = pListUI->GetString();
	Ptr<GTexture> pTexture = GAssetManager::GetInst()->FindAsset<GTexture>(ToWString(KeyName));

	m_Atlas = pTexture;
}

void SpriteEditorUI::Reset()
{
	m_Atlas = nullptr;
	m_RowCol = Vector2(1, 1);
	m_LeftTop = Vector2();
	m_Slice = Vector2(1, 1);
	m_Gap = Vector2();
	m_SpriteName = "";
}

void SpriteEditorUI::InputSpriteValue()
{
	char buffer[255];

	// ��������Ʈ ���� �Է��� ���ʿ� ��ġ�Ѵ�.
	// �ڽĿ������ν� ������ �����Ѵ�.
	// ���� �𸣰� ũ�⸦ �� ���ؾ��� ������ �������� �и��� �Ǿ���.
	ImGui::BeginChild("ChildL", ImVec2(305, ImGui::GetContentRegionAvail().y));

	// =====================
	// Texture �̹���
	ImTextureID ImID = 0;
	ImVec2 uv_min = ImVec2(0, 0);                 // Top-left
	ImVec2 uv_max = ImVec2(1, 1);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
	ImVec2 Ratio = ImVec2(300.f, 300.f);			

	if (m_Atlas != nullptr)
	{
		ImID = (ImTextureID)m_Atlas->GetSRV().Get();
		Ratio.y = 300.f * m_Atlas->GetHeight() / m_Atlas->GetWidth();		// ���̴� �׻� 300 �����̸� �ؽ��� ������ �°� ���̰� �ٲ��.
	}

	ImGui::Image(ImID // �������� �ؽ��� ShaderResourceView
		, Ratio			   // �ؽ��� ���� ũ��
		, uv_min, uv_max				   // ����� �ؽ��� UV ����
		, tint_col, border_col);		   // ƾƮ, ���� �÷�

	string AtlasKey = m_Atlas == nullptr ? "" : ToString(m_Atlas->GetKey());
	ImGui::Text("Atlas");
	ImGui::SameLine(100);
	strcpy_s(buffer, 255, AtlasKey.c_str());
	ImGui::SetNextItemWidth(165);
	ImGui::InputText("##Atlas", buffer, 255, ImGuiInputTextFlags_ReadOnly);

	// Drop Check
	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI���� �帣���� �����͸� �޴´�.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// ���޵� �����Ϳ� ����ִ� Asset�� �ּҰ��� �����´�.
			GAsset* pAsset = *((GAsset**)payload->Data);

			// ������ ������ Mesh Ÿ���� ��쿡�� �۾��� �̾��.
			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				Ptr<GTexture> pTex = dynamic_cast<GTexture*>(pAsset);
				assert(pTex);
				m_Atlas = pTex;
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("...##Texture", ImVec2(28.f, 0.f)))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::TEXTURE, Items);
		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&SpriteEditorUI::SelectTexture);
	}

	// =====================
	// Sprite ����
	ImGui::Text("RowCol");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat2("##RowCol", m_RowCol, 1.f, 1.f, FLT_MAX);

	ImGui::Text("LeftTop");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat2("##LeftTop", m_LeftTop, 1.f, 0.f, FLT_MAX);

	ImGui::Text("Slice");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat2("##Slice", m_Slice, 1.f, 1.f, FLT_MAX);

	// ��׶���� �׻� Slice �̻��̿��� �Ѵ�.
	m_Background.x = m_Background.x < m_Slice.x ? m_Slice.x : m_Background.x;
	m_Background.y = m_Background.y < m_Slice.y ? m_Slice.y : m_Background.y;
	ImGui::Text("Background");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat2("##Background", m_Background, 1.f, 0.f, FLT_MAX);

	// ���� ��������Ʈ ���̰�
	ImGui::Text("Gap");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat2("##Gap", m_Gap, 1.f, 0.f, FLT_MAX);

	// ��������Ʈ �̸�
	ImGui::Text("Sprite Name");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	strcpy_s(buffer, 255, m_SpriteName.c_str());
	if (ImGui::InputText("##Sprite Name", buffer, 255))
	{
		m_SpriteName = buffer;
	}

	// ���� ��ư
	if (ImGui::Button("Create Sprite"))
	{
		ImGui::OpenPopup("Create Sprite PopUP");
	}

	if (ImGui::BeginPopup("Create Sprite PopUP"))
	{
		ImGui::Text("Create Sprite.");
		ImGui::Text("Are You Sure?");
		if (ImGui::Button("OK"))
		{
			CreateSprite();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("NO"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::EndChild();
}

void SpriteEditorUI::PreviewSprite()
{

	// �׵θ� ����, ���� ��ũ�ѹ� �߰�
	ImGui::BeginChild("ChildR", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);

	// ��
	for (int Row = 0; Row < m_RowCol.x; ++Row)
	{
		// ��
		for (int Col = 0; Col < m_RowCol.y; ++Col)
		{
			Vector2 CurLeftTop;
			CurLeftTop.x = m_LeftTop.x + m_Slice.x * Col + (Col == 0 ? 0 : m_Gap.x) * Col;
			CurLeftTop.y = m_LeftTop.y + m_Slice.y * Row + (Row == 0 ? 0 : m_Gap.y) * Row;

			ImTextureID ImID = 0;
			ImVec2 uv_min = ImVec2(0, 0);                 // Top-left
			ImVec2 uv_max = ImVec2(1, 1);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
			ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
			ImVec2 Ratio = ImVec2(100.f, 100.f);		

			if (m_Atlas != nullptr)
			{
				uv_min = ImVec2(CurLeftTop.x / m_Atlas->GetWidth(), CurLeftTop.y / m_Atlas->GetHeight());
				uv_max = ImVec2(uv_min.x + m_Slice.x / m_Atlas->GetWidth(), uv_min.y + m_Slice.y / m_Atlas->GetHeight());

				ImID = (ImTextureID)m_Atlas->GetSRV().Get();
				Ratio.y = 100.f * m_Slice.y / m_Slice.x;	// ���̴� 100 �����̸� ���̴� ������ ���� ���Ѵ�.
			}

			ImGui::Image(ImID // �������� �ؽ��� ShaderResourceView
				, Ratio			   // �ؽ��� ���� ũ��
				, uv_min, uv_max				   // ����� �ؽ��� UV ����
				, tint_col, border_col);		   // ƾƮ, ���� �÷�


			if (Col + 1 < m_RowCol.y)
				ImGui::SameLine();
		}


	}
	ImGui::EndChild();
	
}

void SpriteEditorUI::CreateSprite()
{
	if (m_Atlas == nullptr)
	{
		MessageBox(nullptr, L"�ؽ��İ� �����ϴ�.", L"Sprite ���� �Ұ�", MB_OK);
		return;
	}
	else if (m_SpriteName == "")
	{
		MessageBox(nullptr, L"��������Ʈ �̸��� �����ϴ�.", L"Sprite ���� �Ұ�", MB_OK);
		return;
	}

	int Index = 0;
	wstring SpriteName = L"";
	Ptr<GSprite> pSprite = nullptr;
	wchar_t buffer[255];

	// ��
	for (int Row = 0; Row < m_RowCol.x; ++Row)
	{
		// ��
		for (int Col = 0; Col < m_RowCol.y; ++Col)
		{
			swprintf_s(buffer, 255, L"Sprite\\%ls_%04d.sprite", ToWString(m_SpriteName).c_str(), Index++);
			SpriteName = buffer;

			// Ȥ�� �̹� �ִ� ����̶�� �޽����ڽ�
			if (GAssetManager::GetInst()->FindAsset<GSprite>(SpriteName) != nullptr)
			{
				MessageBox(nullptr, L"�̹� �����ϴ� Ű���Դϴ�.", L"Sprite ���� �Ұ�", MB_OK);
				return;
			}

			pSprite = new GSprite;
			GAssetManager::GetInst()->AddAsset(SpriteName, pSprite.Get());

			Vector2 CurMin, CurMax;
			CurMin.x = m_LeftTop.x + m_Slice.x * Col + (Col == 0 ? 0 : m_Gap.x) * Col;
			CurMin.y = m_LeftTop.y + m_Slice.y * Row + (Row == 0 ? 0 : m_Gap.y) * Row;

			pSprite->SetAtlasTex(m_Atlas);
			pSprite->SetLeftTop(CurMin);
			pSprite->SetSlice(m_Slice);
			//pSprite->SetOffset();
			pSprite->SetBackGround(m_Background);

			// ������ ��ο� ���� ���� �߰�
			wstring Path = GPathManager::GetContentPath() + SpriteName;
			pSprite->Save(Path);
		}

	}
}
