#include "pch.h"
#include "TileRenderUI.h"

#include <Engine/GAssetManager.h>
#include <Engine/GTexture.h>
#include <Engine/GSprite.h>
#include <Engine/GTransform.h>

#include <Engine/GTileRender.h>

#include <Engine/GKeyManager.h>
#include <Engine/GLevelManager.h>

#include "ImGuiManager.h"
#include "ListUI.h"

TileRenderUI::TileRenderUI()
	:RenderComponentUI(COMPONENT_TYPE::TILE_RENDER)
	, m_TileRender(nullptr)
{

}

TileRenderUI::~TileRenderUI()
{

}

void TileRenderUI::Render_UI()
{
	// ��ȿ���� �ʴ� ������Ʈ�ų� ������Ʈ�� ���� ��
	if (!IsValid(GetTarget()) || GetTarget()->TileRender() == nullptr)
	{
		SetActive(false);
		return;
	}

	m_TileRender = GetTarget()->TileRender();
	OutputTitle("Tile Render");
	RenderComponentUI::Render_UI();

	int RowCol[2] = { m_TileRender->GetRow(),m_TileRender->GetCol() };
	ImGui::Text("RowCol");
	ImGui::SameLine(GetTab());
	if (ImGui::DragInt2("##RowCol", RowCol, 1, 1, 64))
	{
		m_TileRender->SetRowCol(RowCol[0], RowCol[1]);
	}
	AddItemHeight();

	Vector2 TileSize = m_TileRender->GetTileSize();
	ImGui::Text("TileSize");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat2("##TileSize", TileSize,1, 0, FLT_MAX))
	{
		m_TileRender->SetTileSize(TileSize);
	}
	AddItemHeight();

	ImGui::BeginDisabled(true);
	Vector2 MousePos = GKeyManager::GetInst()->GetGameMousePos();
	ImGui::Text("Mouse");
	ImGui::SameLine(GetTab());
	ImGui::InputFloat2("##MousePos", MousePos, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::EndDisabled();
	AddItemHeight();

	PreviewCurTile();

	for (int i = 0; i < (int)TILE_PALETTE_PARAM::END; ++i)
	{
		TilePalette((TILE_PALETTE_PARAM)i);
	}

	PreviewTiles();

	// ũ��
	ImGui::Text("IsCollider");
	ImGui::SameLine(GetTab());
	bool bCollider = m_TileRender->IsCollider();
	if (ImGui::Checkbox("##Collider", &bCollider))
	{
		m_TileRender->SetCollider(bCollider);
	}
	AddItemHeight();
}

void TileRenderUI::Update_UI()
{
	RenderComponentUI::Update_UI();

	// ��ȿ���� �ʴ� ������Ʈ�ų� ������Ʈ�� ���� ��
	if (!IsValid(GetTarget()) || GetTarget()->TileRender() == nullptr)
	{
		SetActive(false);
		return;
	}

	if (KEY_PRESSED(KEY::LBTN) || KEY_PRESSED(KEY::RBTN))
	{
		// ȭ���� ����ٸ� ��ȿó��
		if (GKeyManager::GetInst()->IsMouseOffScreen() || GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::STOP)
			return;

		// ���콺 ��ǥ�� Object�� ��ǥ�� ���� Offset�� ���Ѵ�.
		Vector3 vObjectPos = m_TileRender->Transform()->GetWorldPos();
		Vector3 vObjectScale = (m_TileRender->Transform()->GetWorldScale() / 2);
		vObjectPos -= vObjectScale;
		Vector2 MousePos = GKeyManager::GetInst()->GetGameMousePos();
		Vector2 Offset = MousePos - Vector2(vObjectPos.x, vObjectPos.y);

		// �̷��� ���� Offset�� ������� Ÿ�ϸ��� ��� ���̹Ƿ� ��ȿó���Ѵ�.
		if (Offset.x < 0 || Offset.y < 0)
			return;

		// Ÿ�� ũ��
		int Row = Offset.y / (m_TileRender->GetTileSize().x);
		int Col = Offset.x / (m_TileRender->GetTileSize().y);

		// Row�� Col�� Ÿ�ϸ� ���� ũ�ٸ� ��� ���̹Ƿ� ��ȿó���Ѵ�.
		if (Col < 0 || Row < 0 || m_TileRender->GetCol() <= Col || m_TileRender->GetRow() <= Row)
			return;

		// UV�� ������ �Ʒ���
		// ���� ���� �������־�� ��
		Row = m_TileRender->GetRow() - (Row + 1);

		// �� Ÿ���� ���� ��
		if (!m_DrawTile.Draw || KEY_PRESSED(KEY::RBTN))
		{
			m_TileRender->SetTile(Row, Col);
			return;
		}
		
		m_TileRender->SetTile(Row, Col, m_DrawTile);
	}
}

void TileRenderUI::Activate()
{
	m_CurTP = TILE_PALETTE_PARAM::END;
	m_DrawTile = tTileInfo();
}

void TileRenderUI::SelectTP(DWORD_PTR _UI)
{
	ListUI* pListUI = (ListUI*)_UI;
	string KeyName = pListUI->GetString();
	Ptr<GTilePalette> pTP = GAssetManager::GetInst()->FindAsset<GTilePalette>(ToWString(KeyName));

	m_TileRender->SetTilePalette(m_CurTP, pTP);
	m_DrawTile = tTileInfo();

}

void TileRenderUI::TilePalette(TILE_PALETTE_PARAM _Param)
{
	
	char buffer[255];
	sprintf_s(buffer, 255, "Tile Palette : %d", (int)_Param);

	Ptr<GTilePalette> pTP = m_TileRender->GetTilePalette(_Param);
	string Key = "";
	if (pTP != nullptr)
		Key = ToString(pTP->GetKey());

	sprintf_s(buffer, 255, "Tile Palette : %d", (int)_Param);
	ImGui::Text(buffer);

	ImGui::SameLine(GetTab());
	sprintf_s(buffer, 255, "%s##Tile Palette : %d", Key.c_str(), (int)_Param);
	if (ImGui::Button(buffer, ImVec2(300.f, 0.f)))
	{
		m_CurTP = _Param;
		m_DrawTile = tTileInfo();
	}

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
			if (ASSET_TYPE::TILEPALETTE == pAsset->GetType())
			{
				Ptr<GTilePalette> pTP = dynamic_cast<GTilePalette*>(pAsset);
				assert(pTP);
				m_TileRender->SetTilePalette(_Param, pTP);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	sprintf_s(buffer, 255, "...##Tile Palette Button : %d", (int)_Param);
	if (ImGui::Button(buffer, ImVec2(28.f, 0.f)))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::TILEPALETTE, Items);
		pList->SetItems(Items);

		m_CurTP = _Param;
		pList->AddDelegate(this, (EUI_DELEGATE_1)&TileRenderUI::SelectTP);
	}
	
	AddItemHeight();

}

void TileRenderUI::PreviewCurTile()
{
	// =====================
	// Texture �̹���
	ImTextureID ImID = 0;
	ImVec2 uv_min = ImVec2(0.f,0.f);                 // Top-left
	ImVec2 uv_max = ImVec2(0.f, 0.f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
	ImVec2 Ratio = ImVec2(100.f, 100.f);

	if (m_DrawTile.Draw)
	{
		Ptr<GTilePalette> CurTileTP = m_TileRender->GetTilePalette(m_DrawTile.Palette);
		m_LeftTop = CurTileTP->GetTileLeftTop(m_DrawTile.Row, m_DrawTile.Col);
		m_Slice = CurTileTP->GetSlice();
		uv_min = ImVec2(m_LeftTop.x, m_LeftTop.y);
		uv_max = ImVec2(m_LeftTop.x + m_Slice.x, m_LeftTop.y + m_Slice.y);
		Ratio.y = 100.f * CurTileTP->GetSlice_P().y / CurTileTP->GetSlice_P().x;	// ���̴� 100 �����̸� ���̴� ������ ���� ���Ѵ�.

		if (CurTileTP->GetAtlasTex() != nullptr)
		{
			Ptr<GTexture> pTexture = CurTileTP->GetAtlasTex();
			ImID = (ImTextureID)pTexture->GetSRV().Get();
		}
	}

	ImGui::Indent(GetTab());
	ImGui::Image(ImID // �������� �ؽ��� ShaderResourceView
		, Ratio			   // �ؽ��� ���� ũ��
		, uv_min, uv_max				   // ����� �ؽ��� UV ����
		, tint_col, border_col);		   // ƾƮ, ���� �÷�
	ImGui::Unindent(GetTab());
	AddItemHeight();
}

/// <summary>
/// ���� ���õ� TP�� Ÿ�ϵ��� �����ش�.
/// </summary>
void TileRenderUI::PreviewTiles()
{
	if (m_CurTP == TILE_PALETTE_PARAM::END || m_TileRender->GetTilePalette(m_CurTP) == nullptr)
		return;

	ImGui::BeginChild("Tiles", ImVec2(ImGui::GetContentRegionAvail().x,300), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);

	Ptr<GTilePalette> CurTilePalette = m_TileRender->GetTilePalette(m_CurTP);
	Ptr<GTexture> Atlas = CurTilePalette->GetAtlasTex();

	// ��
	for (int Row = 0; Row < CurTilePalette->GetRow(); ++Row)
	{
		// ��
		for (int Col = 0; Col < CurTilePalette->GetCol(); ++Col)
		{
			Vector2 CurLeftTop = CurTilePalette->GetTileLeftTop(Row, Col);

			ImTextureID ImID = 0;
			ImVec2 uv_min = ImVec2(0, 0);                 // Top-left
			ImVec2 uv_max = ImVec2(1, 1);                 // Lower-right
			ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
			ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
			ImVec2 Ratio = ImVec2(100.f, 100.f);

			if (Atlas != nullptr)
			{
				uv_min = ImVec2(CurLeftTop.x, CurLeftTop.y);
				uv_max = ImVec2(uv_min.x + CurTilePalette->GetSlice().x, uv_min.y + CurTilePalette->GetSlice().y);

				ImID = (ImTextureID)Atlas->GetSRV().Get();
				Ratio.y = 100.f * CurTilePalette->GetSlice_P().y / CurTilePalette->GetSlice_P().x;	// ���̴� 100 �����̸� ���̴� ������ ���� ���Ѵ�.
			}

			ImGui::Image(ImID // �������� �ؽ��� ShaderResourceView
				, Ratio			   // �ؽ��� ���� ũ��
				, uv_min, uv_max				   // ����� �ؽ��� UV ����
				, tint_col, border_col);		   // ƾƮ, ���� �÷�
			if (ImGui::IsItemClicked())
			{
				m_DrawTile.Draw = true;
				m_DrawTile.Palette = m_CurTP;
				m_DrawTile.Row = Row;
				m_DrawTile.Col = Col;
			}

			if (Col + 1 < CurTilePalette->GetCol())
				ImGui::SameLine();
		}


	}
	ImGui::EndChild();
	AddItemHeight();
}


