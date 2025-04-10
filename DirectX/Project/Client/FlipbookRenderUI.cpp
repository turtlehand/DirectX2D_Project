#include "pch.h"
#include "FlipbookRenderUI.h"

#include <Engine/GLevelManager.h>

#include <Engine/GAssetManager.h>
#include <Engine/GFlipbook.h>

#include <Engine/GFlipbookRender.h>

#include "ListUI.h"
#include "ImGuiManager.h"

FlipbookRenderUI::FlipbookRenderUI()
	:RenderComponentUI(COMPONENT_TYPE::FLIPBOOK_RENDER)
	, m_FlipbookRender(nullptr)
	, m_FlipbookIdx(0)
{

}

FlipbookRenderUI::~FlipbookRenderUI()
{
}

void FlipbookRenderUI::Render_UI()
{
	// 유효하지 않는 오브젝트거나 컴포넌트가 없을 때
	if (!IsValid(GetTarget()) || GetTarget()->FlipbookRender() == nullptr)
	{
		SetActive(false);
		return;
	}
	m_FlipbookRender = GetTarget()->FlipbookRender();
	OutputTitle("Flipbook Render");
	RenderComponentUI::Render_UI();

	char buffer[255];

	// ===============
	// 반복 버튼
	bool IsRepeat = m_FlipbookRender->GetRepeat();
	ImGui::Text("Loop");
	ImGui::SameLine(GetTab());
	if (ImGui::Checkbox("##Repeat", &IsRepeat))
	{
		m_FlipbookRender->SetRepeat(IsRepeat);
	}
	AddItemHeight();

	// ==========================
	// 현재 플립북
	ImGui::Text("CurFlipbook");
	// 플립북 1개 이상 있을 때만
	if (0 < m_FlipbookRender->GetFlipbookCount())
	{
		// 현재 선택된 플립북 인덱스
		int Index = m_FlipbookRender->GetCurIndex();

		// 현재 선택된 대상이 없다면 "" 삽입
		
		string CurFlipName = Index == -1 ? "" : ToString(m_FlipbookRender->GetFlipbook(Index)->GetKey());
		ImGui::SameLine(GetTab());
		ImGui::SetNextItemWidth(300);
		// Combo 박스
		if (ImGui::BeginCombo("##CurFlipbook", CurFlipName.c_str()))
		{
			for (int i = 0; i < m_FlipbookRender->GetFlipbookCount(); ++i)
			{
				// 인덱스의 플립북 키값
				string FlipName = ToString(m_FlipbookRender->GetFlipbook(i)->GetKey()) + "##" + to_string(i);

				// 현재 선택된 플립북 인덱스가 플립북과 같다면
				bool is_selected = (m_FlipbookRender->GetCurIndex() == i);

				// 플립북을 선택하였다면 그 플립북 재생
				// string을 사용하기 위해 Selectable을 사용
				if (ImGui::Selectable(FlipName.c_str(), is_selected))
					m_FlipbookRender->Play(i, IsRepeat);

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		
		// 일시중지 ,다시 재생 버튼
		ImGui::SameLine();
		string Pause = m_FlipbookRender->GetPause() ? "Play" : "Pause";
		if(ImGui::Button(Pause.c_str()))
		{
			m_FlipbookRender->Pause();
		}
	}
	AddItemHeight();

	// =========================
	// 플립북 Tree
	if(ImGui::TreeNode("Flipbook List"))
	{
		// ============
		// Drop Check
		// Stop 상태에서만 Drop 체크
		if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP)
		{
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
					if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
					{
						GFlipbook* pFlipbook = dynamic_cast<GFlipbook*>(pAsset);
						assert(pFlipbook);
						m_FlipbookRender->AddFlipBook(pFlipbook);
					}
				}

				ImGui::EndDragDropTarget();
			}

		}

		// ===============
		// 플립북 추가 버튼
		ImGui::BeginDisabled(GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::STOP);
		if (ImGui::Button("AddFlipbook##FilpbookList"))
		{
			ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
			assert(pList);

			pList->SetActive(true);

			vector<string> Items;
			GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::FLIPBOOK, Items);
			pList->SetItems(Items);

			pList->AddDelegate(this, (EUI_DELEGATE_1)&FlipbookRenderUI::SelectFlipbook);
			m_FlipbookIdx = -1;		// 현재 Flipbook 초기화
		}
		ImGui::EndDisabled();
		AddItemHeight();


		// 플립북 List 본격적으로 띄우기
		FlipbookList();

		ImGui::TreePop();
	}
	else
	{
		// Stop 상태에서만 Drop Check
		if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP)
		{
			if (ImGui::BeginDragDropTarget())
			{
				// ContentUI에서 드르개된 데이터만 받는다.
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

				if (nullptr != payload)
				{
					// 전달된 데이터에 들어있는 Asset의 주소값을 꺼내온다.
					GAsset* pAsset = *((GAsset**)payload->Data);

					// 꺼내온 에셋이 Mesh 타입인 경우에만 작업을 이어간다.
					if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
					{
						GFlipbook* pFlipbook = dynamic_cast<GFlipbook*>(pAsset);
						assert(pFlipbook);
						m_FlipbookRender->AddFlipBook(pFlipbook);
					}
				}

				ImGui::EndDragDropTarget();
			}
		}


	}
	AddItemHeight();

	
	
}

void FlipbookRenderUI::FlipbookList()
{
	char buffer[255];

	// 플립북들을 띄운다.
	for (int i = 0; i < m_FlipbookRender->GetFlipbookCount(); ++i)
	{
		string FlipbookKey = ToString(m_FlipbookRender->GetFlipbook(i)->GetKey());

		// 플립북 인덱스
		sprintf_s(buffer, 255, "Flipbook : %d", i);
		ImGui::Text(buffer);
		ImGui::SameLine(GetTab());

		// 플립북 Key
		sprintf_s(buffer, 255, "##Flipbook%d", i);
		ImGui::SetNextItemWidth(300);
		ImGui::InputText(buffer, (char*)FlipbookKey.c_str(), FlipbookKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

		// Drop Check
		// Stop 일 때만
		if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP)
		{
			// Drop Check
			if (ImGui::BeginDragDropTarget())
			{
				// ContentUI에서 드르개된 데이터만 받는다.
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

				if (nullptr != payload)
				{
					// 전달된 데이터에 들어있는 Asset의 주소값을 꺼내온다.
					GAsset* pAsset = *((GAsset**)payload->Data);

					if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
					{
						GFlipbook* pFlipbook = dynamic_cast<GFlipbook*>(pAsset);
						assert(pFlipbook);
						m_FlipbookRender->SetFlipbook(pFlipbook, i);
					}
				}

				ImGui::EndDragDropTarget();
			}
		}

		// Stop이 아니면 변경 불가, 삭제 불가
		ImGui::BeginDisabled(GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::STOP);
		// List Button
		ImGui::SameLine();
		sprintf_s(buffer, 255, "...##Filpbook%d", i);
		if (ImGui::Button(buffer))
		{
			ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
			assert(pList);

			pList->SetActive(true);

			vector<string> Items;
			GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::FLIPBOOK, Items);
			pList->SetItems(Items);

			pList->AddDelegate(this, (EUI_DELEGATE_1)&FlipbookRenderUI::SelectFlipbook);
			m_FlipbookIdx = i;
		}

		// Flipbook Delete Button
		ImGui::SameLine();
		sprintf_s(buffer, 255, "Delete##%d", i);
		if (ImGui::Button(buffer))
		{
			sprintf_s(buffer, 255, "Delete Flipbook##%d", i);
			ImGui::OpenPopup(buffer);
		}

		// Delet Cheeck Popup
		sprintf_s(buffer, 255, "Delete Flipbook##%d", i);
		if (ImGui::BeginPopup(buffer))
		{
			ImGui::Text("Delete this Flipbook.");
			ImGui::Text("Are You Sure?");
			if (ImGui::Button("OK"))
			{
				m_FlipbookRender->RemoveFlipbbok(i);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("NO"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndDisabled();


		AddItemHeight();
	}
	
}

void FlipbookRenderUI::SelectFlipbook(DWORD_PTR _UI)
{
	ListUI* pList = (ListUI*)_UI;
	Ptr<GFlipbook> pAsset = GAssetManager::GetInst()->FindAsset<GFlipbook>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));

	if (m_FlipbookIdx == -1)
	{
		m_FlipbookRender->AddFlipBook(pAsset);
		return;
	}

	m_FlipbookRender->SetFlipbook(pAsset, m_FlipbookIdx);
	m_FlipbookIdx = -1;
}


