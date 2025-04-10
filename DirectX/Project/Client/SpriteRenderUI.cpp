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
	// 유효하지 않는 오브젝트거나 컴포넌트가 없을 때
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
		// ContentUI에서 드르개된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// 전달된 데이터에 들어있는 Asset의 주소값을 꺼내온다.
			GAsset* pAsset = *((GAsset**)payload->Data);

			// 꺼내온 에셋이 Mesh 타입인 경우에만 작업을 이어간다.
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

	// 텍스쳐가 없다면 0
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
	ImGui::Image(ImID // 렌더링할 텍스쳐 ShaderResourceView
		, ImVec2(200.f, 200.f)			   // 텍스쳐 위젯 크기
		, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
		, tint_col, border_col);		   // 틴트, 보더 컬러
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