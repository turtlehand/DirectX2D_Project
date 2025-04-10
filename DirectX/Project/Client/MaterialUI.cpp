#include "pch.h"
#include "MaterialUI.h"

#include "ListUI.h"

#include <Engine/GAssetManager.h>
#include <Engine/GMaterial.h>
#include <Engine/GGraphicShader.h>
#include <Engine/GTexture.h>

MaterialUI::MaterialUI()
	:AssetUI(ASSET_TYPE::MATERIAL)
	, m_TexParam(TEX_PARAM::END)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::Init()
{
}

void MaterialUI::Update_UI()
{
}

void MaterialUI::Render_UI()
{
	OutputTitle("Material");
	OutputKeyPath();

	Material();
	Shader();
	ImGui::NewLine();
	ImGui::NewLine();

	ImGui::Text("Material Parameter");
	Scalar_Parameter();
	Texture_Parameter();
}

void MaterialUI::Material()
{
}

void MaterialUI::Shader()
{
	Ptr<GMaterial> pMtrl = (GMaterial*)GetAsset().Get();

	// Shader
	Ptr<GGraphicShader> pShader = pMtrl->GetShader();
	string Key;
	if (pShader != nullptr)
	{
		Key = string().assign(pShader->GetKey().begin(), pShader->GetKey().end());
	}
	ImGui::Text("Graphic Shader");
	ImGui::SameLine(GetTab());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##GraphicShaderName", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

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
			if (ASSET_TYPE::GRAPHIC_SHADER == pAsset->GetType())
			{
				GGraphicShader* pGS = dynamic_cast<GGraphicShader*>(pAsset);
				assert(pGS);
				pMtrl->SetShader(pGS);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("...##GraphicShader", ImVec2(28.f, 0.f)))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::GRAPHIC_SHADER, Items);
		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&MaterialUI::SelectShader);
	}
}

void MaterialUI::Scalar_Parameter()
{
	Ptr<GMaterial> pMtrl = (GMaterial*)GetAsset().Get();
	Ptr<GGraphicShader> pGS = pMtrl->GetShader();

	if (pGS == nullptr)
		return;

	const vector<tScalarParam>& vecScalar = pGS->GetScalarParam();
	const vector<tTexParam>& vecTex = pGS->GetTexParam();

	for (size_t i = 0; i < vecScalar.size(); ++i)
	{
		string label = "##Input" + to_string(i);
		switch (vecScalar[i].Type)
		{
		case SCALAR_PARAM::INT_0:
		case SCALAR_PARAM::INT_1:
		case SCALAR_PARAM::INT_2:
		case SCALAR_PARAM::INT_3:
		{
			string Desc = string(vecScalar[i].Desc.begin(), vecScalar[i].Desc.end());
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(GetTab());

			int Input = pMtrl->GetScalarParam<int>(vecScalar[i].Type);
			if (ImGui::InputInt(label.c_str(), &Input))
			{
				pMtrl->SetScalarParam(vecScalar[i].Type, Input);
			}
		}
			break;
		case SCALAR_PARAM::FLOAT_0:
		case SCALAR_PARAM::FLOAT_1:
		case SCALAR_PARAM::FLOAT_2:
		case SCALAR_PARAM::FLOAT_3:
		{
			string Desc = string(vecScalar[i].Desc.begin(), vecScalar[i].Desc.end());
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(GetTab());

			float Input = pMtrl->GetScalarParam<float>(vecScalar[i].Type);
			if (ImGui::InputFloat(label.c_str(), &Input))
			{
				pMtrl->SetScalarParam(vecScalar[i].Type, Input);
			}
		}
			break;
		case SCALAR_PARAM::VEC2_0:
		case SCALAR_PARAM::VEC2_1:
		case SCALAR_PARAM::VEC2_2:
		case SCALAR_PARAM::VEC2_3:
		{
			string Desc = string(vecScalar[i].Desc.begin(), vecScalar[i].Desc.end());
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(GetTab());

			Vector2 Input = pMtrl->GetScalarParam<Vector2>(vecScalar[i].Type);
			if (ImGui::InputFloat2(label.c_str(), Input))
			{
				pMtrl->SetScalarParam(vecScalar[i].Type, Input);
			}
		}
			break;
		case SCALAR_PARAM::VEC4_0:
		case SCALAR_PARAM::VEC4_1:
		case SCALAR_PARAM::VEC4_2:
		case SCALAR_PARAM::VEC4_3:
		{
			string Desc = string(vecScalar[i].Desc.begin(), vecScalar[i].Desc.end());
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(GetTab());

			Vector4 Input = pMtrl->GetScalarParam<Vector4>(vecScalar[i].Type);
			if (ImGui::InputFloat4(label.c_str(), Input))
			{
				pMtrl->SetScalarParam(vecScalar[i].Type, Input);
			}
		}
			break;
		case SCALAR_PARAM::MAT_0:
		case SCALAR_PARAM::MAT_1:
		case SCALAR_PARAM::MAT_2:
		case SCALAR_PARAM::MAT_3:
		{
			string Desc = string(vecScalar[i].Desc.begin(), vecScalar[i].Desc.end());
			ImGui::Text(Desc.c_str());

			Matrix Data = pMtrl->GetScalarParam<Matrix>(vecScalar[i].Type);

			bool bChanged = false;

			label += "Mat";
			string MatLabel;

			ImGui::SameLine(GetTab());

			ImGui::Indent(GetTab());

			MatLabel = label + "0";
			if (ImGui::InputFloat4(MatLabel.c_str(), &Data._11))
				bChanged = true;

			MatLabel = label + "1";
			if (ImGui::InputFloat4(MatLabel.c_str(), &Data._21))
				bChanged = true;

			MatLabel = label + "2";
			if (ImGui::InputFloat4(MatLabel.c_str(), &Data._31))
				bChanged = true;

			MatLabel = label + "3";
			if (ImGui::InputFloat4(MatLabel.c_str(), &Data._41))
				bChanged = true;

			ImGui::Unindent(GetTab());

			if (bChanged)
			{
				pMtrl->SetScalarParam(vecScalar[i].Type, Data);
			}
		}
			break;
		}
	}
}

void MaterialUI::Texture_Parameter()
{
	Ptr<GMaterial> pMtrl = (GMaterial*)GetAsset().Get();
	Ptr<GGraphicShader> pGS = pMtrl->GetShader();

	if (pGS == nullptr)
		return;

	const vector<tTexParam>& vecTex = pGS->GetTexParam();

	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		Ptr<GTexture> pTexture = pMtrl->GetTexture(vecTex[i].Type);

		string Desc = string(vecTex[i].Desc.begin(), vecTex[i].Desc.end());
		ImGui::Text(Desc.c_str());
		ImGui::SameLine(GetTab());

		// 텍스쳐 	
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
		ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

		ImTextureID ImID = 0;
		// 텍스쳐가 없다면 0
		if (pTexture != nullptr)
			ImID = (ImTextureID)pTexture->GetSRV().Get();

		ImGui::Image(ImID // 렌더링할 텍스쳐 ShaderResourceView
			, ImVec2(200.f, 200.f)			   // 텍스쳐 위젯 크기
			, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
			, tint_col, border_col);		   // 틴트, 보더 컬러

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
					pMtrl->SetTexture(vecTex[i].Type, pTex);
				}
			}

			ImGui::EndDragDropTarget();
		}
		
		ImGui::SameLine();
		string TextureLabel = "...##Texture" + to_string(i);
		if (ImGui::Button(TextureLabel.c_str(), ImVec2(28.f, 0.f)))
		{
			ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
			assert(pList);

			pList->SetActive(true);

			vector<string> Items;
			GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::TEXTURE, Items);
			pList->SetItems(Items);

			pList->AddDelegate(this, (EUI_DELEGATE_1)&MaterialUI::SelectTexture);

			m_TexParam = vecTex[i].Type;
		}
	}
}

void MaterialUI::SelectShader(DWORD_PTR _Data)
{
	ListUI* pList = (ListUI*)_Data;
	Ptr<GGraphicShader> pGS = GAssetManager::GetInst()->FindAsset<GGraphicShader>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));
	Ptr<GMaterial> pMtrl = (GMaterial*)GetAsset().Get();

	pMtrl->SetShader(pGS);

	//pMtrl->Save(GPathManager::GetContentPath() + pMtrl->GetRelativePath());
}

void MaterialUI::SelectTexture(DWORD_PTR _UI)
{
	ListUI* pList = (ListUI*)_UI;
	Ptr<GTexture> pTex = GAssetManager::GetInst()->FindAsset<GTexture>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));
	Ptr<GMaterial> pMtrl = (GMaterial*)GetAsset().Get();

	pMtrl->SetTexture(m_TexParam, pTex);
	m_TexParam = TEX_PARAM::END;

	//pMtrl->Save(GPathManager::GetContentPath() + pMtrl->GetRelativePath());
}


