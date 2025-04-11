#include "pch.h"
#include "SpriteUI.h"

#include <Engine/GAssetManager.h>
#include <Engine/GSprite.h>
#include <Engine/GTexture.h>

#include "ListUI.h"
#include "ImGuiManager.h"


SpriteUI::SpriteUI()
	:AssetUI(ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Update_UI()
{
}

void SpriteUI::Render_UI()
{
	m_Sprite = (GSprite*)GetAsset().Get();
	if (m_Sprite == nullptr)
	{
		SetActive(false);
		return;
	}

	OutputTitle("Sprite");
	OutputKeyPath();

	Sprite();
	Texture();

	Vector2  LeftTop = m_Sprite->GetLeftTop_P();
	Vector2  Slice = m_Sprite->GetSlice_P();
	Vector2  Offset = m_Sprite->GetOffset_P();
	Vector2  Background = m_Sprite->GetBackGround_P();

	ImGui::Text("LeftTop");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##LeftTop", LeftTop, 1.f, 0.f, FLT_MAX))
	{
		m_Sprite->SetLeftTop(LeftTop);
	}

	ImGui::Text("Slice");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##Slice", Slice, 1.f, 1.f, FLT_MAX))
	{
		m_Sprite->SetSlice(Slice);
	}

	ImGui::Text("Offset");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##Offset", Offset))
	{
		m_Sprite->SetOffset(Offset);
	}

	ImGui::Text("Background");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##Background", Background, 1.f, 1.f, FLT_MAX))
	{
		m_Sprite->SetBackGround(Background);
	}

}


void SpriteUI::Sprite()
{
	Ptr<GTexture> pTexture = nullptr;
	Vector2 LeftTop;
	Vector2 Slice;
	ImTextureID ImID = 0;
	ImVec2 Ratio = ImVec2(200.f, 200.f);

	// 텍스쳐가 없다면 0
	if (m_Sprite != nullptr)
	{
		pTexture = m_Sprite->GetAtlasTex();
		LeftTop = m_Sprite->GetLeftTop();
		Slice = m_Sprite->GetSlice();
		ImID = (ImTextureID)pTexture->GetSRV().Get();
		Ratio.y = 200.f * m_Sprite->GetSlice_P().y / m_Sprite->GetSlice_P().x;
	}
	ImVec2 uv_min = ImVec2(LeftTop.x, LeftTop.y);                 // Top-left
	ImVec2 uv_max = ImVec2(LeftTop.x + Slice.x, LeftTop.y + Slice.y);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

	ImGui::Indent(GetTab());
	ImGui::Image(ImID // 렌더링할 텍스쳐 ShaderResourceView
		, Ratio			   // 텍스쳐 위젯 크기
		, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
		, tint_col, border_col);		   // 틴트, 보더 컬러
	ImGui::Unindent(GetTab());
}

void SpriteUI::Texture()
{
	Ptr<GTexture> pTexture = m_Sprite->GetAtlasTex();
	string Key = "";
	if (m_Sprite != nullptr)
		Key = ToString(pTexture->GetKey());

	ImGui::Text("Atlas Texture");
	ImGui::SameLine(GetTab());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##TextureName", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// Drop Check
	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI에서 드르개된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// 전달된 데이터에 들어있는 Asset의 주소값을 꺼내온다.
			GAsset* pAsset = *((GAsset**)payload->Data);

			// 꺼내온 에셋이 Mesh 타입인 경우에만 작업을 이어간다.
			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				Ptr<GTexture> pTex = dynamic_cast<GTexture*>(pAsset);
				assert(pTex);
				m_Sprite->SetAtlasTex(pTex);
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

		pList->AddDelegate(this, (EUI_DELEGATE_1)&SpriteUI::SelectTexture);
	}

}

void SpriteUI::SelectTexture(DWORD_PTR _UI)
{
	ListUI* pListUI = (ListUI*)_UI;
	string KeyName = pListUI->GetString();
	Ptr<GTexture> pTexture = GAssetManager::GetInst()->FindAsset<GTexture>(ToWString(KeyName));

	GSprite* pSprite = (GSprite*)GetAsset().Get();
	assert(pSprite != nullptr);

	pSprite->SetAtlasTex(pTexture);
}

