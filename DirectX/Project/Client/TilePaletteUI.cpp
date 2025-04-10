#include "pch.h"
#include "TilePaletteUI.h"

#include <Engine/GAssetManager.h>
#include <Engine/GTexture.h>
#include <Engine/GSprite.h>

#include "ImGuiManager.h"
#include "ListUI.h"

TilePaletteUI::TilePaletteUI()
	:AssetUI(ASSET_TYPE::TILEPALETTE)
{
}

TilePaletteUI::~TilePaletteUI()
{
}

void TilePaletteUI::Update_UI()
{
}

void TilePaletteUI::Render_UI()
{
	m_TP = (GTilePalette*)GetAsset().Get();
	if (m_TP == nullptr)
	{
		SetActive(false);
		return;
	}

	OutputTitle("Tile Palette");
	OutputKeyPath();

	InputTileValue();
	Tile();
	
}

void TilePaletteUI::SelectTexture(DWORD_PTR _UI)
{
	ListUI* pListUI = (ListUI*)_UI;
	string KeyName = pListUI->GetString();
	Ptr<GTexture> pTexture = GAssetManager::GetInst()->FindAsset<GTexture>(ToWString(KeyName));

	m_TP->SetAtlasTex(pTexture);
}

void TilePaletteUI::InputTileValue()
{
	char buffer[255];

	m_Atlas = m_TP->GetAtlasTex();
	m_RowCol = Vector2(m_TP->GetRow(), m_TP->GetCol());
	m_LeftTop = m_TP->GetLeftTop_P();
	m_Gap = m_TP->GetGap_P();
	m_Slice = m_TP->GetSlice_P();

	// 스프라이트 정보 입력은 왼쪽에 배치한다.
	// 자식영역으로써 영역을 구분한다.
	// 왠지 모르게 크기를 딱 정해야지 오른쪽 영역으로 분리가 되었다.

	// =====================
	// Texture 이미지
	ImTextureID ImID = 0;
	ImVec2 uv_min = ImVec2(0, 0);                 // Top-left
	ImVec2 uv_max = ImVec2(1, 1);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
	ImVec2 Ratio = ImVec2(100.f, 100.f);

	if (m_Atlas != nullptr)
	{
		ImID = (ImTextureID)m_Atlas->GetSRV().Get();
		Ratio.y = 100.f * m_Atlas->GetHeight() / m_Atlas->GetWidth();		// 넓이는 항상 300 고정이며 텍스쳐 비율에 맞게 높이가 바뀐다.
	}

	ImGui::Image(ImID // 렌더링할 텍스쳐 ShaderResourceView
		, Ratio			   // 텍스쳐 위젯 크기
		, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
		, tint_col, border_col);		   // 틴트, 보더 컬러

	string AtlasKey = m_Atlas == nullptr ? "" : ToString(m_Atlas->GetKey());
	ImGui::Text("Atlas");
	ImGui::SameLine(GetTab());
	strcpy_s(buffer, 255, AtlasKey.c_str());
	ImGui::InputText("##Atlas", buffer, 255, ImGuiInputTextFlags_ReadOnly);

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
				m_TP->SetAtlasTex(pTex);
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

		pList->AddDelegate(this, (EUI_DELEGATE_1)&TilePaletteUI::SelectTexture);
	}
	
	// =====================
	// Sprite 정보
	ImGui::Text("RowCol");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##RowCol", m_RowCol, 1.f, 1.f, FLT_MAX))
	{
		m_TP->SetRowCol(m_RowCol.x, m_RowCol.y);
	}

	ImGui::Text("LeftTop");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##LeftTop", m_LeftTop, 1.f, 0.f, FLT_MAX))
	{
		m_TP->SetLeftTop(m_LeftTop);
	}

	ImGui::Text("Slice");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##Slice", m_Slice, 1.f, 1.f, FLT_MAX))
	{
		m_TP->SetSlice(m_Slice);
	}

	// 다음 스프라이트 사이값
	ImGui::Text("Gap");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##Gap", m_Gap, 1.f, 0.f, FLT_MAX))
	{
		m_TP->SetGap(m_Gap);
	}

}

void TilePaletteUI::Tile()
{
	m_Atlas = m_TP->GetAtlasTex();
	m_RowCol = Vector2(m_TP->GetRow(), m_TP->GetCol());
	m_LeftTop = m_TP->GetLeftTop();
	m_Gap = m_TP->GetGap();
	m_Slice = m_TP->GetSlice();

	ImGui::BeginChild("Tile", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);

	// 행
	for (int Row = 0; Row < m_RowCol.x; ++Row)
	{
		// 렬
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
				uv_min = ImVec2(CurLeftTop.x, CurLeftTop.y );
				uv_max = ImVec2(uv_min.x + m_Slice.x , uv_min.y + m_Slice.y );

				ImID = (ImTextureID)m_Atlas->GetSRV().Get();
				Ratio.y = 100.f * m_TP->GetSlice_P().y / m_TP->GetSlice_P().x;	// 넓이는 100 고정이며 높이는 비율에 따라 변한다.
			}

			ImGui::Image(ImID // 렌더링할 텍스쳐 ShaderResourceView
				, Ratio			   // 텍스쳐 위젯 크기
				, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
				, tint_col, border_col);		   // 틴트, 보더 컬러


			if (Col + 1 < m_RowCol.y)
				ImGui::SameLine();
		}


	}
	ImGui::EndChild();

}


