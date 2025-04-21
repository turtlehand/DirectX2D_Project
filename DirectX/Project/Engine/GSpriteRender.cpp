#include "pch.h"
#include "GSpriteRender.h"

#include "GAssetManager.h"

#include "GGameObject.h"
#include "GTransform.h"

GSpriteRender::GSpriteRender()
	:GRenderComponent(COMPONENT_TYPE::SPRITE_RENDER)
	, m_Color(Vector4(1.f,1.f,1.f,1.f))
{
	SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh"));
	CreateSpriteMaterial();
}

GSpriteRender::~GSpriteRender()
{

}

void GSpriteRender::CreateSpriteMaterial()
{
	if (GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"SpriteShader") == nullptr)
	{
		// SpriteShader
		Ptr<GGraphicShader> pShader = new GGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\sprite.fx", "VS_SPRITE");
		pShader->CreatePixelShader(L"HLSL\\sprite.fx", "PS_SPRITE");
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
		//pShader->SetRSType(RS_TYPE::CULL_NONE);
		//pShader->SetDSType(DS_TYPE::NO_WRITE);
		//pShader->SetBSType(BS_TYPE::ALPHABLEND);

		// 에디터
		pShader->AddTexParam(TEX_PARAM::TEX_0, L"Atlas");
		pShader->AddScalarParam(SCALAR_PARAM::VEC2_0, L"LeftTop");
		pShader->AddScalarParam(SCALAR_PARAM::VEC2_1, L"Slice");
		pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, L"Color");

		GAssetManager::GetInst()->AddAsset(L"SpriteShader", pShader.Get());
	}

	if (GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"SpriteShader_AlphaBland") == nullptr)
	{
		// SpriteShader
		Ptr<GGraphicShader> pShader = new GGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\sprite.fx", "VS_SPRITE");
		pShader->CreatePixelShader(L"HLSL\\sprite.fx", "PS_SPRITE");
		//pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		//pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDSType(DS_TYPE::NO_WRITE);
		pShader->SetBSType(BS_TYPE::ALPHABLEND);

		// 에디터
		pShader->AddTexParam(TEX_PARAM::TEX_0, L"Atlas");
		pShader->AddScalarParam(SCALAR_PARAM::VEC2_0, L"LeftTop");
		pShader->AddScalarParam(SCALAR_PARAM::VEC2_1, L"Slice");
		pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, L"Color");

		GAssetManager::GetInst()->AddAsset(L"SpriteShader_AlphaBland", pShader.Get());
	}

	if (GAssetManager::GetInst()->FindAsset<GMaterial>(L"SpriteMtrl") == nullptr)
	{
		// SpriteMtrl
		Ptr<GMaterial> pMtrl = new GMaterial;
		pMtrl->SetShader(GAssetManager::GetInst()->FindAsset<GGraphicShader>(L"SpriteShader"));

		GAssetManager::GetInst()->AddAsset(L"SpriteMtrl", pMtrl.Get());
	}

	SetMaterial(GAssetManager::GetInst()->FindAsset<GMaterial>(L"SpriteMtrl"));
}

void GSpriteRender::FinalUpdate()
{

}

void GSpriteRender::Render()
{
	GameObject()->Transform()->Binding();

	if (m_Sprite != nullptr)
	{
		//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, (int)!!m_Sprite->GetAtlasTex().Get());
		GetMaterial()->SetTexture(TEX_PARAM::TEX_0, m_Sprite->GetAtlasTex());
		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, m_Sprite->GetLeftTop());
		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, m_Sprite->GetSlice());
		GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Color);
	}
	else
	{
		//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		GetMaterial()->SetTexture(TEX_PARAM::TEX_0, nullptr);
	}
	GetMaterial()->Binding();


	GetMesh()->Render();


}

void GSpriteRender::SaveToFile(FILE* _File)
{
	GRenderComponent::SaveToFile(_File);
	SaveAssetRef(m_Sprite, _File);
	fwrite(m_Color, sizeof(Vector4), 1, _File);
}

void GSpriteRender::LoadFromFile(FILE* _File)
{
	GRenderComponent::LoadFromFile(_File);
	m_Sprite = LoadAssetRef<GSprite>(_File);
	fread(m_Color, sizeof(Vector4), 1, _File);
}


