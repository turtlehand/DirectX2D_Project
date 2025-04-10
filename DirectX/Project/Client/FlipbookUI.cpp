#include "pch.h"
#include "FlipbookUI.h"

#include <Engine/GTimeManager.h>

#include <Engine/GLevelManager.h>

#include <Engine/GAssetManager.h>
#include <Engine/GFlipbook.h>
#include <Engine/GSprite.h>
#include <Engine/GTexture.h>

#include "ListUI.h"
#include "ImGuiManager.h"

FlipbookUI::FlipbookUI()
	:AssetUI(ASSET_TYPE::FLIPBOOK)
	, m_Flip(nullptr)
	, m_SelectIdx(0)
	, m_AnimationIdx(0)
	, m_Time(0)
	, m_Finish(0)
{
}

FlipbookUI::~FlipbookUI()
{
}

void FlipbookUI::Update_UI()
{
	if (m_Flip == nullptr || m_Finish || m_Flip->GetAnimationCount() < 1)
	{
		return;
	}

	m_Time += ENGINEDT;

	if (m_Flip->GetInfo(m_AnimationIdx).Duration < m_Time)
	{
		m_Time -= m_Flip->GetInfo(m_AnimationIdx).Duration;

		++m_AnimationIdx;

		if (m_Flip->GetAnimationCount() <= m_AnimationIdx)
		{
			m_Finish = true;
			m_AnimationIdx -= 1;
		}
	}
}

void FlipbookUI::Render_UI()
{
	m_Flip = (GFlipbook*)GetAsset().Get();
	if (m_Flip == nullptr)
	{
		SetActive(false);
		return;
	}

	OutputTitle("Flipbook");
	OutputKeyPath();

	if (ImGui::Button("Play"))
	{
		RePlay();
	}

	ImGui::SameLine();
	Image(m_AnimationIdx);
	AnimationList();
}

void FlipbookUI::Activate()
{
	RePlay();
}

/// <summary>
/// �� ���� �ִ� �ִϸ��̼��� �ٽ� ����մϴ�.
/// ���� FlipBook�� ��ȭ�� �ִٸ� ������ ȣ��
/// </summary>
void FlipbookUI::RePlay()
{
	m_Time = 0;
	m_Finish = false;
	m_AnimationIdx = 0;
}

/// <summary>
/// AddSprit ��ư�� �߰��ϸ� Animation ����Ʈ�� ���ϴ�.
/// </summary>
void FlipbookUI::AnimationList()
{
	// Sprite �߰� ��ư
	// ���� �÷����� �� ���� �߰� �Ұ���
	ImGui::BeginDisabled(GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::PLAY);
	if (ImGui::Button("AddSprite"))
	{
		m_Flip->AddAnimation();
		RePlay();
	}
	ImGui::EndDisabled();

	char buffer[255];
	for (int i = 0; i < m_Flip->GetAnimationCount(); ++i)
	{
		string SpriteKey = "";
		string SpriteName = "";
		if (m_Flip->GetSprite(i) != nullptr)
		{
			SpriteKey = ToString(m_Flip->GetSprite(i)->GetKey());
			SpriteName = path(ToString(m_Flip->GetSprite(i)->GetKey())).stem().string();
		}

		SpriteName += "##" + to_string(i);

		if (ImGui::TreeNode(SpriteName.c_str()))
		{
			Sprite(i, SpriteKey);
			Info(i);

			sprintf_s(buffer, 255, "Delete##Animation : %d", i);
			ImGui::BeginDisabled(GLevelManager::GetInst()->GetCurrentLevelState() == LEVEL_STATE::PLAY);
			if (ImGui::Button(buffer))
			{
				ImGui::OpenPopup("Delete Animation");
			}

			if (ImGui::BeginPopup("Delete Animation"))
			{
				ImGui::Text("Delete this Animation.");
				ImGui::Text("Are You Sure?");
				if (ImGui::Button("OK"))
				{
					m_Flip->RemoveAnimation(i);
					RePlay();
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


			ImGui::TreePop();
		}
		else
		{
			// Drop Check
			// Tree Node�� Drop ������� ����
			DropCheckSprite(i);
		}
	}

}

/// <summary>
/// �ش� �ε����� Sprite�� ���ϴ�.
/// </summary>
/// <param name="_Idx"></param>
/// <param name="_SpriteKey"></param>
void FlipbookUI::Sprite(int _Idx, const string& _SpriteKey)
{
	ImGui::Text("Sprite");
	ImGui::SameLine(GetTab());
	ImGui::InputText("##SpriteKey", (char*)_SpriteKey.c_str(), _SpriteKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// Drop Check
	DropCheckSprite(_Idx);

	// List Button
	ImGui::SameLine();
	ListButtonSprite(_Idx);

	// Sprite Image
	Image(_Idx);

}

/// <summary>
/// �ش� �ε����� Info�� ���ϴ�.
/// </summary>
/// <param name="_Idx"></param>
/// <param name="_SpriteKey"></param>
void FlipbookUI::Info(int _Idx)
{
	tAnimationInfo tInfo = m_Flip->GetInfo(_Idx);
	bool IsChange = false;

	ImGui::Text("Animation Transform");
	ImGui::SameLine(GetTab());
	if (ImGui::Checkbox("##Animation Transform", &(tInfo.AnimationTransform)))
	{
		IsChange = true;
	}

	ImGui::BeginDisabled(!tInfo.AnimationTransform);

	ImGui::Text("Position");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Position", tInfo.Position))
	{
		IsChange = true;
	}

	ImGui::Text("Scale");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Scale", tInfo.Scale))
	{
		IsChange = true;
	}

	ImGui::Text("Rotation");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat3("##Rotation", tInfo.Rotation))
	{
		IsChange = true;
	}
	ImGui::EndDisabled();

	ImGui::Text("Color");
	ImGui::SameLine(GetTab());
	if (ImGui::ColorEdit4("##Color", tInfo.Color))
	{
		IsChange = true;
	}

	ImGui::Text("Duration");
	ImGui::SameLine(GetTab());
	if (ImGui::DragFloat("##Duration", &(tInfo.Duration), 0.1f, 0.0f, 255.f))
	{
		IsChange = true;
	}

	if (IsChange)
	{
		m_Flip->SetInfo(tInfo, _Idx);
		RePlay();
	}
}


/// <summary>
/// �ش� �ε����� �̹����� ���ϴ�.
/// </summary>
/// <param name="_Idx"></param>
void FlipbookUI::Image(int _Idx)
{
	// Sprite Image
	Ptr<GTexture> pTexture = nullptr;
	Vector2 LeftTop;
	Vector2 Slice;
	Vector4 Color;
	ImVec2 Ratio = ImVec2(200.f, 200.f);
	ImTextureID ImID = 0;
	Ptr<GSprite> pSprite = 0 < m_Flip->GetAnimationCount() ? m_Flip->GetSprite(_Idx) : nullptr;

	// �ؽ��İ� ���ٸ� 0
	if (pSprite != nullptr)
	{
		pTexture = pSprite->GetAtlasTex();
		LeftTop = pSprite->GetLeftTop();
		Slice = pSprite->GetSlice();
		Color = m_Flip->GetInfo(_Idx).Color;
		Ratio.x = 200.f * Slice.x / Slice.y;
		ImID = (ImTextureID)pTexture->GetSRV().Get();
	}
	ImVec2 uv_min = ImVec2(LeftTop.x, LeftTop.y);                 // Top-left
	ImVec2 uv_max = ImVec2(LeftTop.x + Slice.x, LeftTop.y + Slice.y);                 // Lower-right
	ImVec4 tint_col = ImVec4(Color.x, Color.y, Color.z, Color.w);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

	ImGui::Indent(GetTab());
	ImGui::Image(ImID // �������� �ؽ��� ShaderResourceView
		, Ratio			   // �ؽ��� ���� ũ��
		, uv_min, uv_max				   // ����� �ؽ��� UV ����
		, tint_col, border_col);		   // ƾƮ, ���� �÷�
	ImGui::Unindent(GetTab());
}

/// <summary>
/// ��������Ʈ�� ListUI�� ���õ� ���¹��ڿ��� ������ �����մϴ�.
/// </summary>
/// <param name="_UI"></param>
void FlipbookUI::SelectSprite(DWORD_PTR _UI)
{
	ListUI* pList = (ListUI*)_UI;
	Ptr<GSprite> pSprite = GAssetManager::GetInst()->FindAsset<GSprite>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));

	m_Flip->SetSprite(pSprite, m_SelectIdx);
	RePlay();
}

/// <summary>
/// �ε������� �ٷ� ���� �ִ� ������ Drop Ÿ���� �Ǹ� Drop �� Sprite�� ����˴ϴ�.
/// </summary>
/// <param name="_Idx"></param>
void FlipbookUI::DropCheckSprite(int _Idx)
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
			if (ASSET_TYPE::SPRITE == pAsset->GetType())
			{
				GSprite* pSpr = dynamic_cast<GSprite*>(pAsset);
				assert(pSpr);
				m_Flip->SetSprite(pSpr, _Idx);
			}
		}

		RePlay();
		ImGui::EndDragDropTarget();
	}
}


/// <summary>
/// �ش� �ε����� ��������Ʈ�� ������ �� �ִ� ListUI�� ���ϴ�.
/// </summary>
/// <param name="_Idx"></param>
void FlipbookUI::ListButtonSprite(int _Idx)
{
	if (ImGui::Button("...##Sprite"))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::SPRITE, Items);
		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&FlipbookUI::SelectSprite);
		m_SelectIdx = _Idx;
	}
}


