#include "pch.h"
#include "GRenderManager.h"

#include "GAssetManager.h"

#include "components.h"
#include "assets.h"

void GRenderManager::Init()
{
	// 포스트 프로세스 용도 텍스쳐 생성
	Ptr<GTexture> pRTTex = GAssetManager::GetInst()->FindAsset<GTexture>(L"RenderTargetTex");
	m_PostProcessTex = GAssetManager::GetInst()->CreateTexture(L"PostProcessTex"
		, pRTTex->GetWidth(), pRTTex->GetHeight()
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	// =================
	// DebugShapeShader
	// =================
	Ptr<GGraphicShader> pShader = new GGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\debug_shape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"HLSL\\debug_shape.fx", "PS_DebugShape");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WIRTE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	GAssetManager::GetInst()->AddAsset(L"DebugShapeShader", pShader.Get());

	// =================
	// DebugLineShader
	// =================
	pShader = new GGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\debug_shape.fx", "VS_DebugLine");
	pShader->CreateGeometryShader(L"HLSL\\debug_shape.fx", "GS_DebugLine");
	pShader->CreatePixelShader(L"HLSL\\debug_shape.fx", "PS_DebugShape");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WIRTE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);		// 점이 들어간다고 알려줌

	GAssetManager::GetInst()->AddAsset(L"DebugLineShader", pShader.Get());

	// ======================
	// DebugShapeMtrl
	// ======================

	Ptr<GMaterial> pMtrl = new GMaterial;
	pMtrl->SetShader(pShader);
	GAssetManager::GetInst()->AddAsset(L"DeBugShapeMtrl", pMtrl.Get());

	m_DebugObject = new GGameObject;
	m_DebugObject->AddComponent<GTransform>();
	m_DebugObject->AddComponent<GMeshRender>();
	m_DebugObject->Renderer()->SetMaterial(pMtrl);

}