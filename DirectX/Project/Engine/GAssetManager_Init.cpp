#include "pch.h"
#include "GAssetManager.h"
#include "assets.h"
#include "GPathManager.h"

void GAssetManager::Init()
{
	CreateEngineRectMesh();
	CreateEngineGraphicShader();
	//CreateEngineTexture();
	CreateEngineComputeShader();
	CreateEngineMaterial();
	//CreateEngineSprite();
}

void GAssetManager::CreateEngineRectMesh()
{
	// ========
	// RectMesh
	// ========
	{
		// 0 -- 1
		// | \  |
		// 3 -- 2
		Ptr<GMesh> pMesh = new GMesh(true);
		pMesh->SetName(L"RectMesh");

		Vertex arrVtx[4] = {};
		arrVtx[0].vPos = Vector3(-0.5f, 0.5f, 0.f);
		arrVtx[0].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[0].vUV = Vector2(0.f, 0.f);

		arrVtx[1].vPos = Vector3(0.5f, 0.5f, 0.f);
		arrVtx[1].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[1].vUV = Vector2(1.f, 0.f);

		arrVtx[2].vPos = Vector3(0.5f, -0.5f, 0.f);
		arrVtx[2].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[2].vUV = Vector2(1.f, 1.f);

		arrVtx[3].vPos = Vector3(-0.5f, -0.5f, 0.f);
		arrVtx[3].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[3].vUV = Vector2(0.f, 1.f);

		// Index
		UINT arrIdx[6] = { 0,1,2,0,2,3 };
		pMesh->Create(arrVtx, 4, arrIdx, 6);

		AddAsset(L"RectMesh", pMesh.Get());
	}

	// ========
	// RectMesh_Debug
	// ========
	{
		// 0 -- 1
		// |    |
		// 3 -- 2
		Ptr<GMesh> pMesh = new GMesh(true);
		pMesh->SetName(L"RectMesh_Debug");

		Vertex arrVtx[4] = {};
		arrVtx[0].vPos = Vector3(-0.5f, 0.5f, 0.f);
		arrVtx[0].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[0].vUV = Vector2(0.f, 0.f);

		arrVtx[1].vPos = Vector3(0.5f, 0.5f, 0.f);
		arrVtx[1].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[1].vUV = Vector2(1.f, 0.f);

		arrVtx[2].vPos = Vector3(0.5f, -0.5f, 0.f);
		arrVtx[2].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[2].vUV = Vector2(1.f, 1.f);

		arrVtx[3].vPos = Vector3(-0.5f, -0.5f, 0.f);
		arrVtx[3].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		arrVtx[3].vUV = Vector2(0.f, 1.f);

		// Index
		UINT arrIdx[5] = { 0,1,2,3,0 };
		pMesh->Create(arrVtx, 4, arrIdx, 5);

		AddAsset(L"RectMesh_Debug", pMesh.Get());
	}


	// ========
	// CircleMesh
	// ========
	{

		Ptr<GMesh> pMesh = new GMesh(true);
		pMesh->SetName(L"CircleMesh");

		vector<Vertex> vecVtx;
		vector<UINT> vecIdx;

		int Slice = 24;
		float Radius = 0.5f;
		float Angle = 0.f;

		Vertex Center;
		Center.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		Center.vPos = Vector3(0.f, 0.f, 0.f);
		Center.vUV = Vector2(0.5f, 0.5f);
		vecVtx.push_back(Center);

		for (int i = 0; i < Slice + 1; ++i)
		{
			Vertex vtx;
			vtx.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
			vtx.vPos = Vector3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
			vtx.vUV = Vector2(0.5f + Radius * cosf(Angle), (1.f - (Radius * sinf(Angle) + 0.5f)));
			vecVtx.push_back(vtx);

			Angle = Angle + XM_PI * 2 / Slice;
		}

		for (int i = 0; i < Slice; ++i)
		{
			vecIdx.push_back(0);
			vecIdx.push_back(i + 2);
			vecIdx.push_back(i + 1);
		}


		pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());

		AddAsset(L"CircleMesh", pMesh.Get());
	}

	// ========
	// CircleMesh_Debug
	// ========
	{

		Ptr<GMesh> pMesh = new GMesh(true);
		pMesh->SetName(L"CircleMesh");

		vector<Vertex> vecVtx;
		vector<UINT> vecIdx;

		int Slice = 48;
		float Radius = 0.5f;
		float Angle = 0.f;

		for (int i = 0; i < Slice + 1; ++i)
		{
			Vertex vtx;
			vtx.vColor = Vector4(0.f, 0.f, 0.f, 1.f);
			vtx.vPos = Vector3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
			vtx.vUV = Vector2(0.5f + Radius * cosf(Angle), (1.f - (Radius * sinf(Angle) + 0.5f)));
			vecVtx.push_back(vtx);

			Angle = Angle + XM_PI * 2 / Slice;
		}

		for (size_t i = 0; i < vecVtx.size() - 1; ++i)
		{
			vecIdx.push_back(i + 1);
		}


		pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());

		AddAsset(L"CircleMesh_Debug", pMesh.Get());
	}

	// ========
	// PointMesh
	// ========
	{
		Ptr<GMesh> pMesh = new GMesh(true);
		Vertex vPoint;
		vPoint.vPos = Vector3(0.f, 0.f, 0.f);
		vPoint.vUV = Vector2(0.f, 0.f);
		vPoint.vColor = Vector4(1.f, 1.f, 1.f,1.f);

		UINT idx = 0;

		pMesh->Create(&vPoint, 1, &idx, 1);
		AddAsset(L"PointMesh", pMesh.Get());
	}

	// ========
	// LineMesh
	// ========
	{

		Ptr<GMesh> pMesh = new GMesh(true);
		pMesh->SetName(L"LineMesh");

		Vertex Vtx[2] = {};

		// 중심
		Vtx[0].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		Vtx[0].vPos = Vector3(0.f, 0.f, 0.f);
		Vtx[0].vUV = Vector2(0.5f, 0.5f);

		// X축
		Vtx[1].vColor = Vector4(0.f, 0.f, 0.f, 1.f);
		Vtx[1].vPos = Vector3(0.5f, 0.f, 0.f);
		Vtx[1].vUV = Vector2(1.f, 0.5f);

		UINT Idx[2] = {0, 1};

		pMesh->Create(Vtx, 2, Idx, 2);

		AddAsset(L"LineMesh", pMesh.Get());
	}

}

void GAssetManager::CreateEngineTexture()
{
	//============================
	// 텍스쳐 로딩하기
	//============================

	Load<GTexture>(L"Texture\\Character.png", L"Texture\\Character.png");

	Load<GTexture>(L"Texture\\Idle_Left.bmp", L"Texture\\Idle_Left.bmp");

	Load<GTexture>(L"Texture\\particle\\Bubbles99px.png", L"Texture\\particle\\Bubbles99px.png");

	Load<GTexture>(L"Texture\\link.png", L"Texture\\link.png");

	Load<GTexture>(L"Texture\\noise\\noise_01.png", L"Texture\\noise\\noise_01.png");

	Load<GTexture>(L"Texture\\noise\\noise_02.png", L"Texture\\noise\\noise_02.png");

	Load<GTexture>(L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg");
}

void GAssetManager::CreateEngineSprite()
{
	
	wstring ContentPath = GPathManager::GetContentPath();

	// Link Sprite
	Ptr<GSprite> pSprite = nullptr;

	Vector2 LeftTopPixel = Vector2(0.f, 520.f);
	Vector2 SlicePixel = Vector2(120.f, 130.f);
	int FrmCount = 10;

	for (int i = 0; i < FrmCount; ++i)
	{
		pSprite = new GSprite(true);
		pSprite->SetAtlasTex(GetInst()->Load<GTexture>(L"Texture\\link.png", L"Texture\\link.png"));
		pSprite->SetLeftTop(LeftTopPixel + Vector2(120.f * i, 0.f));
		pSprite->SetSlice(SlicePixel);
		pSprite->SetBackGround(Vector2(300.f, 300.f));

		wchar_t szName[255] = {};
		swprintf_s(szName, 255, L"Sprite\\LinkSprite_%d.sprite", i);

		AddAsset(szName, pSprite.Get());

		pSprite->Save(ContentPath + pSprite->GetKey());
	}


	Ptr<GFlipbook> pFlipbook = new GFlipbook;

	tAnimationInfo AInfo = { false, Vector3(), Vector3(), Vector3(), Vector4(1.f,1.f,1.f,1.f),1.f };

	for (int i = 0; i < FrmCount; ++i)
	{
		wchar_t szName[255] = {};
		swprintf_s(szName, 255, L"Sprite\\LinkSprite_%d.sprite", i);
		tAnimationInfo AInfo = { false, Vector3(), Vector3(), Vector3(), Vector4(1.f,1.f,1.f,1.f),1.f };
		pFlipbook->AddAnimation(FindAsset<GSprite>(szName), AInfo);
	}

	AddAsset(L"Flipbook\\LinkFlipbook.flip", pFlipbook.Get());
	pFlipbook->Save(ContentPath + pFlipbook->GetKey());


	// Tile Sprite
	Ptr<GTexture> pAtlas = GAssetManager::GetInst()->Load<GTexture>(L"Texture\\TILE.bmp", L"Texture\\TILE.bmp");

	for (int Row = 0; Row < 6; ++Row)
	{
		for (int Col = 0; Col < 8; ++Col)
		{
			pSprite = new GSprite(true);
			pSprite->SetAtlasTex(pAtlas);
			pSprite->SetLeftTop(Vector2(Col * 64.f, Row * 64.f));
			pSprite->SetSlice(Vector2(64.f, 64.f));

			// 0 ~ 47 까지 뒤에 숫자를 붙인다.
			int Idx = Row * 8 + Col;
			wchar_t szName[255] = {};
			swprintf_s(szName, 255, L"Sprite\\TileSprite_%d.sprite", Idx);
			AddAsset(szName, pSprite.Get());
			pSprite->Save(ContentPath + pSprite->GetKey());
		}
	}
	
}

void GAssetManager::CreateEngineGraphicShader()
{
	Ptr<GGraphicShader> pGS = new GGraphicShader;

	pGS->CreateVertexShader(L"HLSL\\Std2d.fx", "VS_Std2D");
	pGS->CreatePixelShader(L"HLSL\\Std2d.fx", "PS_Std2D");
	pGS->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pGS->SetRSType(RS_TYPE::CULL_NONE);
	pGS->SetBSType(BS_TYPE::DEFAULT);

	pGS->AddTexParam(TEX_PARAM::TEX_0, L"Texture");
	pGS->AddScalarParam(SCALAR_PARAM::VEC4_0, L"Color");


	AddAsset(L"Std2DShader", pGS.Get());

	// =====================
	// Std2DAlphaBlendShader
	// =====================
	pGS = new GGraphicShader;

	pGS->CreateVertexShader(L"HLSL\\Std2d.fx", "VS_Std2D");
	pGS->CreatePixelShader(L"HLSL\\Std2d.fx", "PS_Std2D_AlphaBlend");
	pGS->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pGS->SetRSType(RS_TYPE::CULL_NONE);
	pGS->SetDSType(DS_TYPE::NO_WRITE);
	pGS->SetBSType(BS_TYPE::ALPHABLEND);

	pGS->AddTexParam(TEX_PARAM::TEX_0, L"Texture");
	AddAsset(L"Std2DAlphaBlendShader", pGS.Get());

	// =====================
	// Greater
	// =====================
	pGS = new GGraphicShader;

	pGS->CreateVertexShader(L"HLSL\\Std2d.fx", "VS_Std2D");
	pGS->CreatePixelShader(L"HLSL\\Std2d.fx", "PS_Std2D");
	pGS->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pGS->SetRSType(RS_TYPE::CULL_NONE);
	pGS->SetDSType(DS_TYPE::GREATER);
	pGS->SetBSType(BS_TYPE::DEFAULT);

	AddAsset(L"Std2DGreater", pGS.Get());

	// PostProcess 쉐이더
	pGS = new GGraphicShader;

	pGS->CreateVertexShader(L"HLSL\\postprocess.fx", "VS_PostProcess");
	pGS->CreatePixelShader(L"HLSL\\postprocess.fx", "PS_PostProcess");
	pGS->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pGS->SetRSType(RS_TYPE::CULL_NONE);
	pGS->SetDSType(DS_TYPE::NO_TEST_NO_WIRTE);
	pGS->SetBSType(BS_TYPE::DEFAULT);

	AddAsset(L"PostProcessShader", pGS.Get());

	// 볼록렌즈 쉐이더
	pGS = new GGraphicShader;

	pGS->CreateVertexShader(L"HLSL\\postprocess.fx", "VS_Convex");
	pGS->CreatePixelShader(L"HLSL\\postprocess.fx", "PS_Convex");
	pGS->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pGS->SetRSType(RS_TYPE::CULL_NONE);
	pGS->SetDSType(DS_TYPE::NO_WRITE);
	pGS->SetBSType(BS_TYPE::DEFAULT);

	AddAsset(L"ConvexShader", pGS.Get());

	// 울렁임 쉐이더
	pGS = new GGraphicShader;

	pGS->CreateVertexShader(L"HLSL\\postprocess.fx", "VS_Distortion");
	pGS->CreatePixelShader(L"HLSL\\postprocess.fx", "PS_Distortion");
	pGS->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pGS->SetRSType(RS_TYPE::CULL_NONE);
	pGS->SetDSType(DS_TYPE::NO_WRITE);
	pGS->SetBSType(BS_TYPE::DEFAULT);

	pGS->AddScalarParam(SCALAR_PARAM::VEC4_0, L"Color");
	pGS->AddTexParam(TEX_PARAM::TEX_1, L"Noise");

	AddAsset(L"DistortionShader", pGS.Get());
}

void GAssetManager::CreateEngineComputeShader()
{
}

void GAssetManager::CreateEngineMaterial()
{
	// Std2DMaterial

	Ptr<GMaterial> pMtrl = new GMaterial(true);
	pMtrl->SetShader(FindAsset<GGraphicShader>(L"Std2DShader").Get());
	pMtrl->SetTexture(TEX_PARAM::TEX_0, Load<GTexture>(L"Texture\\Default.png", L"Texture\\Default.png").Get());
	AddAsset(L"Std2DMtrl", pMtrl.Get());

	// PostProcessMtrl
	pMtrl = new GMaterial(true);
	pMtrl->SetShader(FindAsset<GGraphicShader>(L"PostProcessShader"));
	AddAsset(L"PostProcessMtrl", pMtrl.Get());

	// Convex
	// 볼록 렌즈
	// 상 크게
	pMtrl = new GMaterial(true);
	pMtrl->SetShader(FindAsset<GGraphicShader>(L"ConvexShader"));
	AddAsset(L"ConvexMtrl", pMtrl.Get());

	// Distortion
	pMtrl = new GMaterial(true);
	pMtrl->SetShader(FindAsset<GGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexture(TEX_PARAM::TEX_1, Load<GTexture>(L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg").Get());
	AddAsset(L"DistortionMtrl", pMtrl.Get());
}

