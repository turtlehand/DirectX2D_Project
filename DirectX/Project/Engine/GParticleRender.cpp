#include "pch.h"
#include "GParticleRender.h"

#include "GTransform.h"
#include "GStructuredBuffer.h"

#include "GAssetManager.h"
#include "GTimeManager.h"

GParticleRender::GParticleRender()
	: GRenderComponent(COMPONENT_TYPE::PARTICLE_RENDER)
	, m_ParticleBuffer(nullptr)
	, m_SpawnBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_TickCS(nullptr)
	, m_MaxParticle(1000)
	, m_Module{}
	, m_ModuleChanged(false)
	, m_AccTime(0.f)
	, m_ParticleTexture(nullptr)
	
{
	m_SpawnBuffer = new GStructuredBuffer;
	m_SpawnBuffer->Create(sizeof(Vector4), 1, SB_TYPE::SRV_UAV, true);						// Gpu에서 수정, 따라서 SRV_UAV 동시

	m_ModuleBuffer = new GStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_ONLY, true);			// ComputeShader에서 수정하지 않음, CPU - GPU 정보 이동은 있음

	m_ParticleBuffer = new GStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false);	// Gpu에서 수정, 따라서 SRV_UAV 동시, CPU - GPU 정보 이동 없음

	// 메쉬 설정
	if (nullptr == GetMesh())
		SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh"));

	CreateMtrl();

	// 파티클 Tick 컴퓨트 섀디어
	Ptr<GComputeShader> pCS = GAssetManager::GetInst()->FindAsset<GComputeShader>(L"ParticleTickCS");
	if (pCS == nullptr)
	{
		pCS = new GParticleTickCS;
		pCS->SetName(L"ParticleTickCS");
		GAssetManager::GetInst()->AddAsset(pCS->GetName(), pCS.Get());
	}
	m_TickCS = (GParticleTickCS*)pCS.Get();
}

GParticleRender::GParticleRender(const GParticleRender& _Origin) 
	: GRenderComponent(_Origin)
	, m_ParticleBuffer(nullptr)
	, m_SpawnBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_TickCS(_Origin.m_TickCS)		// ComputerShader가 에셋이라
	, m_MaxParticle(_Origin.m_MaxParticle)
	, m_Module(_Origin.m_Module)
	, m_ModuleChanged(false)
	, m_AccTime(0.f)
	, m_ParticleTexture(_Origin.m_ParticleTexture)
{
	m_SpawnBuffer = new GStructuredBuffer;
	m_SpawnBuffer->Create(sizeof(Vector4), 1, SB_TYPE::SRV_UAV, true);						// Gpu에서 수정, 따라서 SRV_UAV 동시

	m_ModuleBuffer = new GStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_ONLY, true);			// ComputeShader에서 수정하지 않음, CPU - GPU 정보 이동은 있음

	m_ParticleBuffer = new GStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false);	// Gpu에서 수정, 따라서 SRV_UAV 동시, CPU - GPU 정보 이동 없음

	// 파티클 Tick 컴퓨트 섀디어
	Ptr<GComputeShader> pCS = GAssetManager::GetInst()->FindAsset<GComputeShader>(L"ParticleTickCS");
	if (pCS == nullptr)
	{
		pCS = new GParticleTickCS;
		pCS->SetName(L"ParticleTickCS");
		GAssetManager::GetInst()->AddAsset(pCS->GetName(), pCS.Get());
	}
	m_TickCS = (GParticleTickCS*)pCS.Get();
}

GParticleRender::~GParticleRender()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
	if (nullptr != m_SpawnBuffer)
		delete m_SpawnBuffer;
	if (nullptr != m_ModuleBuffer)
		delete m_ModuleBuffer;
}

void GParticleRender::FinalUpdate()
{
	m_ModuleChanged = true;
	m_Module.ObjectWorldPos = Transform()->GetWorldPos();

	// 이번 프레임 파티클 활성화 개수 계산
	CalcSpawnCount();

	// 파티클 모듈 변경점 반영
	if (m_ModuleChanged)
	{
		m_ModuleChanged = false;
		m_ModuleBuffer->SetData(&m_Module);
	}

	// 컴퓨트 셰이더 설정
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnBuffer(m_SpawnBuffer);
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->SetNoiseTex(GAssetManager::GetInst()->Load<GTexture>(L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg"));

	m_TickCS->Excute();
}

void GParticleRender::Render()
{
	// 바인딩을 해야 View, Proj 행렬을 넘겨줌
	Transform()->Binding();

	// 파티클 버퍼 바인딩
	m_ParticleBuffer->Binding(20);

	// 파티클 버퍼 바인딩
	m_ModuleBuffer->Binding(21);

	// 재질 및 섀이더 바인딩
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Transform()->GetWorldPos());
	GetMaterial()->SetTexture(TEX_PARAM::TEX_0, m_ParticleTexture);
	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);

	GetMaterial()->Binding();

	// 파이클 인스턴싱 렌더링
	GetMesh()->Render_Particle(m_MaxParticle);

	m_ParticleBuffer->Clear(20);
	m_ModuleBuffer->Clear(21);
}

void GParticleRender::CreateMtrl()
{
	
	if (nullptr != GetMaterial())
		return;

	// 재질이 없다면 찾음
	Ptr<GMaterial> pMtrl = GAssetManager::GetInst()->FindAsset<GMaterial>(L"ParticleMtrl");

	// 재질이 생성되지 않았다면 새로 생성
	if (nullptr != pMtrl)
	{
		SetMaterial(pMtrl);
		return;
	}
		

	// 섀이더 생성
	Ptr<GGraphicShader> pShader = new GGraphicShader;
	pShader->SetName(L"ParticleRenderShader");

	pShader->CreateVertexShader(L"HLSL\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"HLSL\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"HLSL\\particle.fx", "PS_Particle");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);		// 점이 들어간다고 알려줌

	GAssetManager::GetInst()->AddAsset(pShader->GetName(), pShader.Get());


	// 재질 생성
	pMtrl = new GMaterial;
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(pShader);
	GAssetManager::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());

	// 재질 설정
	SetMaterial(pMtrl);
}

void GParticleRender::CalcSpawnCount()
{
	m_AccTime += ENGINEDT;
	float Term = 1.f / m_Module.SpawnRate;

	tSpawnCount count;

	if (Term < m_AccTime)
	{
		m_AccTime -= Term;

		count.SpawnCount = 1;
	}

	if (m_Module.Module[(int)PARTICLE_MODULE::SPAWN_BURST] && 0  < m_Module.SpawnBurstRepeat)
	{
		static float AccTime = 0.f;
		AccTime += ENGINEDT;

		if (m_Module.SpawnBurstRepeatTime < AccTime)
		{
			count.SpawnCount += m_Module.SpawnBurstCount;

			m_Module.SpawnBurstRepeat -= 1;
			AccTime -= m_Module.SpawnBurstRepeatTime;
		}
	}

	if (0 < count.SpawnCount)
	{
		m_SpawnBuffer->SetData(&count);
	}
}

void GParticleRender::SaveToFile(FILE* _File)
{
	GRenderComponent::SaveToFile(_File);

	SaveAssetRef(m_TickCS, _File);
	SaveAssetRef(m_ParticleTexture, _File);
	fwrite(&m_MaxParticle, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(tParticleModule), 1, _File);
}

void GParticleRender::LoadFromFile(FILE* _File)
{
	GRenderComponent::LoadFromFile(_File);

	m_TickCS = (GParticleTickCS*)LoadAssetRef<GComputeShader>(_File).Get();
	SetParticleTex(LoadAssetRef<GTexture>(_File));
	fread(&m_MaxParticle, sizeof(UINT), 1, _File);
	fread(&m_Module, sizeof(tParticleModule), 1, _File);
}