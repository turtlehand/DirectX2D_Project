#include "pch.h"
#include "SpriteRenderUI.h"

#include <Engine/GSpriteRender.h>

#include <Engine/GAssetManager.h>
#include <Engine/GSprite.h>
#include <Engine/GTexture.h>

#include "ListUI.h"


SpriteRenderUI::SpriteRenderUI()
	:RenderComponentUI(COMPONENT_TYPE::SPRITE_RENDER)
	, m_SpriteRender(nullptr)
{
}

SpriteRenderUI::~SpriteRenderUI()
{
}

void SpriteRenderUI::Render_UI()
{
	// ��ȿ���� �ʴ� ������Ʈ�ų� ������Ʈ�� ���� ��
	if (!IsValid(GetTarget()) || GetTarget()->SpriteRender() == nullptr)
	{
		SetActive(false);
		return;
	}
	m_SpriteRender = GetTarget()->SpriteRender();
	OutputTitle("Sprite Render");
	RenderComponentUI::Render_UI();

	// LightColor
	Vector4 vColor = m_SpriteRender->GetColor();
	ImGui::Text("Color");
	ImGui::SameLine(GetTab());
	if (ImGui::ColorEdit4("##vColor", vColor))
	{
		m_SpriteRender->SetColor(vColor);
	}
	AddItemHeight();

	// Sprite
	Ptr<GSprite> pSprite = m_SpriteRender->GetSprite();
	string Key = "";
	if (pSprite != nullptr)
		Key = string().assign(pSprite->GetKey().begin(), pSprite->GetKey().end());
	ImGui::Text("Sprite");
	ImGui::SameLine(GetTab());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##SpriteName", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

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
			if (ASSET_TYPE::SPRITE == pAsset->GetType())
			{
				GSprite* pSpr = dynamic_cast<GSprite*>(pAsset);
				assert(pSpr);
				m_SpriteRender->SetSprite(pSpr);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("...##Sprite"))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::SPRITE, Items);
		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&SpriteRenderUI::SelectSprite);
	}
	AddItemHeight();

	Ptr<GTexture> pTexture = nullptr;
	Vector2 LeftTop;
	Vector2 Slice;
	ImTextureID ImID = 0;

	// �ؽ��İ� ���ٸ� 0
	if (pSprite != nullptr)
	{
		pTexture = pSprite->GetAtlasTex();
		LeftTop = pSprite->GetLeftTop();
		Slice = pSprite->GetSlice();
		ImID = (ImTextureID)pTexture->GetSRV().Get();
	}
	ImVec2 uv_min = ImVec2(LeftTop.x, LeftTop.y);                 // Top-left
	ImVec2 uv_max = ImVec2(LeftTop.x + Slice.x, LeftTop.y + Slice.y);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

	ImGui::Indent(GetTab());
	ImGui::Image(ImID // �������� �ؽ��� ShaderResourceView
		, ImVec2(200.f, 200.f)			   // �ؽ��� ���� ũ��
		, uv_min, uv_max				   // ����� �ؽ��� UV ����
		, tint_col, border_col);		   // ƾƮ, ���� �÷�
	ImGui::Unindent(GetTab());
	AddItemHeight();
}

void SpriteRenderUI::SelectSprite(DWORD_PTR _UI)
{
	ListUI* pList = (ListUI*)_UI;
	Ptr<GSprite> pSprite = GAssetManager::GetInst()->FindAsset<GSprite>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));
	GSpriteRender* pSpriteRender = GetTarget()->SpriteRender();

	pSpriteRender->SetSprite(pSprite);
}