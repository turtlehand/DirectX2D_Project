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

	// 스프라이트 정보 입력은 왼쪽에 배치한다.
	// 자식영역으로써 영역을 구분한다.
	// 왠지 모르게 크기를 딱 정해야지 오른쪽 영역으로 분리가 되었다.
	ImGui::BeginChild("ChildL", ImVec2(305, ImGui::GetContentRegionAvail().y));

	// =====================
	// Texture 이미지
	ImTextureID ImID = 0;
	ImVec2 uv_min = ImVec2(0, 0);                 // Top-left
	ImVec2 uv_max = ImVec2(1, 1);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
	ImVec2 Ratio = ImVec2(300.f, 300.f);			

	if (m_Atlas != nullptr)
	{
		ImID = (ImTextureID)m_Atlas->GetSRV().Get();
		Ratio.y = 300.f * m_Atlas->GetHeight() / m_Atlas->GetWidth();		// 넓이는 항상 300 고정이며 텍스쳐 비율에 맞게 높이가 바뀐다.
	}

	ImGui::Image(ImID // 렌더링할 텍스쳐 ShaderResourceView
		, Ratio			   // 텍스쳐 위젯 크기
		, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
		, tint_col, border_col);		   // 틴트, 보더 컬러

	string AtlasKey = m_Atlas == nullptr ? "" : ToString(m_Atlas->GetKey());
	ImGui::Text("Atlas");
	ImGui::SameLine(100);
	strcpy_s(buffer, 255, AtlasKey.c_str());
	ImGui::SetNextItemWidth(165);
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
	// Sprite 정보
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

	// 백그라운드는 항상 Slice 이상이여야 한다.
	m_Background.x = m_Background.x < m_Slice.x ? m_Slice.x : m_Background.x;
	m_Background.y = m_Background.y < m_Slice.y ? m_Slice.y : m_Background.y;
	ImGui::Text("Background");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat2("##Background", m_Background, 1.f, 0.f, FLT_MAX);

	// 다음 스프라이트 사이값
	ImGui::Text("Gap");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat2("##Gap", m_Gap, 1.f, 0.f, FLT_MAX);

	// 스프라이트 이름
	ImGui::Text("Sprite Name");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(200);
	strcpy_s(buffer, 255, m_SpriteName.c_str());
	if (ImGui::InputText("##Sprite Name", buffer, 255))
	{
		m_SpriteName = buffer;
	}

	// 생성 버튼
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

	// 테두리 있음, 수평 스크롤바 추가
	ImGui::BeginChild("ChildR", ImGui::GetContentRegionAvail(), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);

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
				uv_min = ImVec2(CurLeftTop.x / m_Atlas->GetWidth(), CurLeftTop.y / m_Atlas->GetHeight());
				uv_max = ImVec2(uv_min.x + m_Slice.x / m_Atlas->GetWidth(), uv_min.y + m_Slice.y / m_Atlas->GetHeight());

				ImID = (ImTextureID)m_Atlas->GetSRV().Get();
				Ratio.y = 100.f * m_Slice.y / m_Slice.x;	// 넓이는 100 고정이며 높이는 비율에 따라 변한다.
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

void SpriteEditorUI::CreateSprite()
{
	if (m_Atlas == nullptr)
	{
		MessageBox(nullptr, L"텍스쳐가 없습니다.", L"Sprite 생성 불가", MB_OK);
		return;
	}
	else if (m_SpriteName == "")
	{
		MessageBox(nullptr, L"스프라이트 이름이 없습니다.", L"Sprite 생성 불가", MB_OK);
		return;
	}

	int Index = 0;
	wstring SpriteName = L"";
	Ptr<GSprite> pSprite = nullptr;
	wchar_t buffer[255];

	// 행
	for (int Row = 0; Row < m_RowCol.x; ++Row)
	{
		// 렬
		for (int Col = 0; Col < m_RowCol.y; ++Col)
		{
			swprintf_s(buffer, 255, L"Sprite\\%ls_%04d.sprite", ToWString(m_SpriteName).c_str(), Index++);
			SpriteName = buffer;

			// 혹시 이미 있는 대상이라면 메시지박스
			if (GAssetManager::GetInst()->FindAsset<GSprite>(SpriteName) != nullptr)
			{
				MessageBox(nullptr, L"이미 존재하는 키값입니다.", L"Sprite 생성 불가", MB_OK);
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

			// 컨텐츠 경로에 저장 파일 추가
			wstring Path = GPathManager::GetContentPath() + SpriteName;
			pSprite->Save(Path);
		}

	}
}
