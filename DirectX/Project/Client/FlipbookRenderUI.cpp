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
	// ��ȿ���� �ʴ� ������Ʈ�ų� ������Ʈ�� ���� ��
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
	// �ݺ� ��ư
	bool IsRepeat = m_FlipbookRender->GetRepeat();
	ImGui::Text("Loop");
	ImGui::SameLine(GetTab());
	if (ImGui::Checkbox("##Repeat", &IsRepeat))
	{
		m_FlipbookRender->SetRepeat(IsRepeat);
	}
	AddItemHeight();

	// ==========================
	// ���� �ø���
	ImGui::Text("CurFlipbook");
	// �ø��� 1�� �̻� ���� ����
	if (0 < m_FlipbookRender->GetFlipbookCount())
	{
		// ���� ���õ� �ø��� �ε���
		int Index = m_FlipbookRender->GetCurIndex();

		// ���� ���õ� ����� ���ٸ� "" ����
		
		string CurFlipName = Index == -1 ? "" : ToString(m_FlipbookRender->GetFlipbook(Index)->GetKey());
		ImGui::SameLine(GetTab());
		ImGui::SetNextItemWidth(300);
		// Combo �ڽ�
		if (ImGui::BeginCombo("##CurFlipbook", CurFlipName.c_str()))
		{
			for (int i = 0; i < m_FlipbookRender->GetFlipbookCount(); ++i)
			{
				// �ε����� �ø��� Ű��
				string FlipName = ToString(m_FlipbookRender->GetFlipbook(i)->GetKey()) + "##" + to_string(i);

				// ���� ���õ� �ø��� �ε����� �ø��ϰ� ���ٸ�
				bool is_selected = (m_FlipbookRender->GetCurIndex() == i);

				// �ø����� �����Ͽ��ٸ� �� �ø��� ���
				// string�� ����ϱ� ���� Selectable�� ���
				if (ImGui::Selectable(FlipName.c_str(), is_selected))
					m_FlipbookRender->Play(i, IsRepeat);

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		
		// �Ͻ����� ,�ٽ� ��� ��ư
		ImGui::SameLine();
		string Pause = m_FlipbookRender->GetPause() ? "Play" : "Pause";
		if(ImGui::Button(Pause.c_str()))
		{
			m_FlipbookRender->Pause();
		}
	}
	AddItemHeight();

	// =========================
	// �ø��� Tree
	if(ImGui::TreeNode("Flipbook List"))
	{
		// ============
		// Drop Check
		// Stop ���¿����� Drop üũ
		if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP)
		{
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
		// �ø��� �߰� ��ư
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
			m_FlipbookIdx = -1;		// ���� Flipbook �ʱ�ȭ
		}
		ImGui::EndDisabled();
		AddItemHeight();


		// �ø��� List ���������� ����
		FlipbookList();

		ImGui::TreePop();
	}
	else
	{
		// Stop ���¿����� Drop Check
		if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP)
		{
			if (ImGui::BeginDragDropTarget())
			{
				// ContentUI���� �帣���� �����͸� �޴´�.
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

				if (nullptr != payload)
				{
					// ���޵� �����Ϳ� ����ִ� Asset�� �ּҰ��� �����´�.
					GAsset* pAsset = *((GAsset**)payload->Data);

					// ������ ������ Mesh Ÿ���� ��쿡�� �۾��� �̾��.
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

	// �ø��ϵ��� ����.
	for (int i = 0; i < m_FlipbookRender->GetFlipbookCount(); ++i)
	{
		string FlipbookKey = ToString(m_FlipbookRender->GetFlipbook(i)->GetKey());

		// �ø��� �ε���
		sprintf_s(buffer, 255, "Flipbook : %d", i);
		ImGui::Text(buffer);
		ImGui::SameLine(GetTab());

		// �ø��� Key
		sprintf_s(buffer, 255, "##Flipbook%d", i);
		ImGui::SetNextItemWidth(300);
		ImGui::InputText(buffer, (char*)FlipbookKey.c_str(), FlipbookKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

		// Drop Check
		// Stop �� ����
		if (GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::STOP)
		{
			// Drop Check
			if (ImGui::BeginDragDropTarget())
			{
				// ContentUI���� �帣���� �����͸� �޴´�.
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

				if (nullptr != payload)
				{
					// ���޵� �����Ϳ� ����ִ� Asset�� �ּҰ��� �����´�.
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

		// Stop�� �ƴϸ� ���� �Ұ�, ���� �Ұ�
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


