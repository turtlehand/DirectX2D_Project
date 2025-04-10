#include "pch.h"
#include "TextureUI.h"

#include <Engine/GTexture.h>

void TextureUI::Init()
{
}

void TextureUI::Update_UI()
{
}

void TextureUI::Render_UI()
{
	OutputTitle("Texture");
	OutputKeyPath();

	Ptr<GTexture> pTex = dynamic_cast<GTexture*>(GetAsset().Get());
	assert(pTex.Get());

	// �ؽ��� 	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

	ImGui::Image((ImTextureID)pTex->GetSRV().Get() // �������� �ؽ��� ShaderResourceView
		, ImVec2(300.f, 300.f)			   // �ؽ��� ���� ũ��
		, uv_min, uv_max				   // ����� �ؽ��� UV ����
		, tint_col, border_col);		   // ƾƮ, ���� �÷�


	//�ؽ��� �ػ�
	char buffer[255];

	int width = pTex->GetWidth();
	int height = pTex->GetHeight();
	
	sprintf_s(buffer, 255, "%d", width);
	ImGui::Text("Width");
	ImGui::SameLine(100);
	ImGui::InputText("##Width", buffer, 255, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(buffer, 255, "%d", height);
	ImGui::Text("Height");
	ImGui::SameLine(100);
	ImGui::InputText("##Height", buffer, 255, ImGuiInputTextFlags_ReadOnly);

	
}

TextureUI::TextureUI()
	:AssetUI(ASSET_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}
