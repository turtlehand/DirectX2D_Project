#include "pch.h"
#include "ParticleRenderUI.h"

#include <Engine/GAssetManager.h>
#include <Engine/GTexture.h>
#include <Engine/GSprite.h>
#include <Engine/GTransform.h>

#include <Engine/GParticleRender.h>

#include "ImGuiManager.h"
#include "ListUI.h"

ParticleRenderUI::ParticleRenderUI()
	: RenderComponentUI(COMPONENT_TYPE::PARTICLE_RENDER)
	, m_ParticleRender(nullptr)
{
}

ParticleRenderUI::~ParticleRenderUI()
{
}

void ParticleRenderUI::Render_UI()
{
	// 유효하지 않는 오브젝트거나 컴포넌트가 없을 때
	if (!IsValid(GetTarget()) || GetTarget()->ParticleRender() == nullptr)
	{
		SetActive(false);
		return;
	}

	m_ParticleRender = GetTarget()->ParticleRender();
	OutputTitle("Particle Render");
	RenderComponentUI::Render_UI();

	Texture();
	Spawn();
	SpawnBurst();
	Velocity();
	Scale();
	Drag();
	NoiseForce();
	RenderModule();
}

void ParticleRenderUI::SelectTexture(DWORD_PTR _UI)
{
	ListUI* pList = (ListUI*)_UI;
	Ptr<GTexture> pTex = GAssetManager::GetInst()->FindAsset<GTexture>(wstring().assign(pList->GetString().begin(), pList->GetString().end()));
	GParticleRender* pParticleRender = GetTarget()->ParticleRender();

	pParticleRender->SetParticleTex(pTex);
}

void ParticleRenderUI::Texture()
{
	Ptr<GTexture> pAtlas = m_ParticleRender->GetParticleTex();

	// GTexture
	string Key = "";
	if (pAtlas != nullptr)
		Key = string().assign(pAtlas->GetKey().begin(), pAtlas->GetKey().end());
	ImGui::Text("Texture");
	ImGui::SameLine(GetTab());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##Texture Name", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

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
				GTexture* pTex = dynamic_cast<GTexture*>(pAsset);
				assert(pTex);
				m_ParticleRender->SetParticleTex(pTex);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("...##Texture List"))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		GAssetManager::GetInst()->GetAssetNameList(ASSET_TYPE::TEXTURE, Items);
		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&ParticleRenderUI::SelectTexture);
	}
	AddItemHeight();


	ImTextureID TexID = 0;
	ImVec2 Size = ImVec2(200.f, 200.f);
	ImVec2 UV_Min = ImVec2(0.f, 0.f);
	ImVec2 UV_Max = ImVec2(1.f, 1.f);
	ImVec4 tint_Col = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 border_Col = ImVec4(0.f, 0.f, 1.f, 1.f);

	if (pAtlas != nullptr)
	{
		TexID = (ImTextureID)(pAtlas->GetSRV().Get());
	}

	ImGui::Indent(GetTab());
	ImGui::Image(TexID,
		Size,
		UV_Min,
		UV_Max,
		tint_Col,
		border_Col);
	ImGui::Unindent(GetTab());
	AddItemHeight();
}

void ParticleRenderUI::Spawn()
{
	// 스폰 모듈

	if (ImGui::TreeNode("Spawn"))
	{
		bool bSpawnModule = m_ParticleRender->GetModule(PARTICLE_MODULE::SPAWN);
		ImGui::Text("Spawn");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##SpawnModule", &bSpawnModule))
		{
			m_ParticleRender->SetModule(PARTICLE_MODULE::SPAWN, bSpawnModule);
		}
		AddItemHeight();

		ImGui::BeginDisabled(!bSpawnModule);

		float fSpawnRate = m_ParticleRender->GetSpawnRate();
		ImGui::Text("Spawn Rate");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Spawn Rate", &fSpawnRate, 0.01f, 0.01f, FLT_MAX))
		{
			m_ParticleRender->SetSpawnRate(fSpawnRate);
		}
		AddItemHeight();

		Vector4 vStartColor = m_ParticleRender->GetStartColor();
		ImGui::Text("Start Color");
		ImGui::SameLine(GetTab());
		if (ImGui::ColorEdit4("##Start Color", vStartColor))
		{
			m_ParticleRender->SetStartColor(vStartColor);
		}
		AddItemHeight();

		Vector3 vMinScale = m_ParticleRender->GetMinScale();
		ImGui::Text("Min Scale");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat3("##Min Scale", vMinScale, 0.1f))
		{
			m_ParticleRender->SetMinScale(vMinScale);
		}
		AddItemHeight();

		Vector3 vMaxScale = m_ParticleRender->GetMaxScale();
		ImGui::Text("Max Scale");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat3("##Max Scale", vMaxScale, 0.1f))
		{
			m_ParticleRender->SetMaxScale(vMaxScale);
		}
		AddItemHeight();

		float fMinLife = m_ParticleRender->GetMinLife();
		ImGui::Text("Min Life");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Min Life", &fMinLife, 0.01f, 0.01f, FLT_MAX))
		{
			m_ParticleRender->SetMinLife(fMinLife);
		}
		AddItemHeight();

		float fMaxLife = m_ParticleRender->GetMaxLife();
		ImGui::Text("Max Life");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Max Life", &fMaxLife, 0.01f, 0.01f, FLT_MAX))
		{
			m_ParticleRender->SetMaxLife(fMaxLife);
		}
		AddItemHeight();

		const char* SpawnShapeItem[] = { "Box", "Sphere" };
		int iSpawnShape = m_ParticleRender->GetSpawnShape();
		ImGui::Text("Spawn Shape");
		ImGui::SameLine(GetTab());
		ImGui::Combo("##Spawn Shape", &iSpawnShape, SpawnShapeItem, IM_ARRAYSIZE(SpawnShapeItem));
		if (iSpawnShape != m_ParticleRender->GetSpawnShape())
			m_ParticleRender->SetSpawnShape(iSpawnShape);
		AddItemHeight();

		Vector3 vSpawnShapeScale = m_ParticleRender->GetSpawnShapeScale();
		ImGui::Text("Shape Scale");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat3("##ShapeScale", vSpawnShapeScale))
		{
			m_ParticleRender->SetSpawnShapeScale(vSpawnShapeScale);
		}
		AddItemHeight();

		const char* SpaceTypeItem[] = { "Local", "World" };
		int iSpaceType = m_ParticleRender->GetSpaceType();
		ImGui::Text("Space Type");
		ImGui::SameLine(GetTab());
		ImGui::Combo("##Space Type", &iSpaceType, SpaceTypeItem, IM_ARRAYSIZE(SpaceTypeItem));
		if (iSpaceType != m_ParticleRender->GetSpaceType())
			m_ParticleRender->SetSpaceType(iSpaceType);
		AddItemHeight();

		ImGui::EndDisabled();
		ImGui::TreePop();
	}
	AddItemHeight();
}

void ParticleRenderUI::SpawnBurst()
{
	// 속도 모듈

	if (ImGui::TreeNode("Spawn Burst"))
	{
		bool bSpawnBurstModule = m_ParticleRender->GetModule(PARTICLE_MODULE::SPAWN_BURST);
		ImGui::Text("Spawn Burst");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##Spawn Burst", &bSpawnBurstModule))
		{
			m_ParticleRender->SetModule(PARTICLE_MODULE::SPAWN_BURST, bSpawnBurstModule);
		}
		AddItemHeight();

		ImGui::BeginDisabled(!bSpawnBurstModule);

		int iParticleCount = m_ParticleRender->GetBurstParticleCount();
		ImGui::Text("Burst Particle Count");
		ImGui::SameLine(GetTab());
		if (ImGui::DragInt("##Burst Particle Count", &iParticleCount, 1, 0, m_ParticleRender->GetMaxParticle()))
		{
			m_ParticleRender->SetBurstParticleCount(iParticleCount);
		}
		AddItemHeight();

		int iRepeatCount = m_ParticleRender->GetBurstRepeatCount();
		ImGui::Text("Burst Repeat Count");
		ImGui::SameLine(GetTab());
		if (ImGui::DragInt("##Burst Repeat Count", &iRepeatCount, 1, 0, INT_MAX))
		{
			m_ParticleRender->SetBurstRepeatCount(iRepeatCount);
		}
		AddItemHeight();

		float fBurstTerm = m_ParticleRender->GetBurstTerm();
		ImGui::Text("Burst Term");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Burst Term", &fBurstTerm, 1, 0, FLT_MAX))
		{
			m_ParticleRender->SetBurstTerm(fBurstTerm);
		}
		AddItemHeight();

		ImGui::EndDisabled();
		ImGui::TreePop();
	}
	AddItemHeight();
}

void ParticleRenderUI::Velocity()
{
	// 속도 모듈

	if (ImGui::TreeNode("Add Velocity"))
	{
		bool bVelocityModule = m_ParticleRender->GetModule(PARTICLE_MODULE::ADD_VELOCITY);
		ImGui::Text("Add Velocity");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##Add Velocity", &bVelocityModule))
		{
			m_ParticleRender->SetModule(PARTICLE_MODULE::ADD_VELOCITY, bVelocityModule);
		}
		AddItemHeight();

		ImGui::BeginDisabled(!bVelocityModule);

		const char* VelocityTypeItem[] = { "Random", "From Center","To Center", "Fixed"};
		int iVelocityType = m_ParticleRender->GetAddVelocityType();
		ImGui::Text("Add Velocity Type");
		ImGui::SameLine(GetTab());
		ImGui::Combo("##Add Velocity Type", &iVelocityType, VelocityTypeItem, IM_ARRAYSIZE(VelocityTypeItem));
		if (iVelocityType != m_ParticleRender->GetAddVelocityType())
			m_ParticleRender->SetAddVelocityType(iVelocityType);
		AddItemHeight();

		float fMinSpeed = m_ParticleRender->GetMinSpeed();
		float fMaxSpeed = m_ParticleRender->GetMaxSpeed();

		ImGui::Text("Min Speed");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Min Speed", &fMinSpeed, 0.01f, 0.01f, fMaxSpeed))
		{
			m_ParticleRender->SetMinSpeed(fMinSpeed);
		}
		AddItemHeight();
		
		ImGui::Text("Max Speed");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Max Speed", &fMaxSpeed, 0.01f, fMinSpeed, FLT_MAX))
		{
			m_ParticleRender->SetMaxSpeed(fMaxSpeed);
		}
		AddItemHeight();

		Vector3 vVelocity = m_ParticleRender->GetFixedVelocity();
		ImGui::Text("Fixed Velocity");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat3("##Fixed Velocity", vVelocity, 0.1f))
		{
			m_ParticleRender->SetFixedVelocity(vVelocity);
		}
		AddItemHeight();

		ImGui::EndDisabled();
		ImGui::TreePop();
	}
	AddItemHeight();
}

void ParticleRenderUI::Scale()
{
	// 크기

	if (ImGui::TreeNode("Scale"))
	{
		bool bScale = m_ParticleRender->GetModule(PARTICLE_MODULE::SCALE);
		ImGui::Text("Scale");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##Scale", &bScale))
		{
			m_ParticleRender->SetModule(PARTICLE_MODULE::SCALE, bScale);
		}
		AddItemHeight();

		ImGui::BeginDisabled(!bScale);

		float fStartScale= m_ParticleRender->GetStartScale();
		float fEndScale = m_ParticleRender->GetEndScale();

		ImGui::Text("Start Scale");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Start Scale", &fStartScale))
		{
			m_ParticleRender->SetStartScale(fStartScale);
		}
		AddItemHeight();

		ImGui::Text("End Scale");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Max Speed", &fEndScale))
		{
			m_ParticleRender->SetEndScale(fEndScale);
		}
		AddItemHeight();

		ImGui::EndDisabled();
		ImGui::TreePop();
	}
	AddItemHeight();
}

void ParticleRenderUI::Drag()
{
	// 크기

	if (ImGui::TreeNode("Drag"))
	{
		bool bDrag = m_ParticleRender->GetModule(PARTICLE_MODULE::DRAG);
		ImGui::Text("Drag");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##Drag", &bDrag))
		{
			m_ParticleRender->SetModule(PARTICLE_MODULE::DRAG, bDrag);
		}
		AddItemHeight();

		ImGui::BeginDisabled(!bDrag);

		float fDragAge = m_ParticleRender->GetDragDestNormalizeAge();
		ImGui::Text("Drag Age");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Drag Age", &fDragAge,0.001,0.001f,1.f))
		{
			m_ParticleRender->SetDragDestNormalizeAge(fDragAge);
		}
		AddItemHeight();

		float fDragLimit = m_ParticleRender->GetDragLimitSpeed();
		ImGui::Text("Drag Min Speed");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Drag Min Speed", &fDragLimit))
		{
			m_ParticleRender->SetDragLimitSpeed(fDragLimit);
		}
		AddItemHeight();

		ImGui::EndDisabled();
		ImGui::TreePop();
	}
	AddItemHeight();
}

void ParticleRenderUI::NoiseForce()
{
	// 크기

	if (ImGui::TreeNode("NoiseForce"))
	{
		bool bNoiseForce = m_ParticleRender->GetModule(PARTICLE_MODULE::NOISE_FORCE);
		ImGui::Text("Noise Force");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##Noise Force", &bNoiseForce))
		{
			m_ParticleRender->SetModule(PARTICLE_MODULE::NOISE_FORCE, bNoiseForce);
		}
		AddItemHeight();

		ImGui::BeginDisabled(!bNoiseForce);

		float fTerm = m_ParticleRender->GetNoiseForceTerm();
		ImGui::Text("Noise Force Term");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Noise Force Term", &fTerm, 0.1, 0.1f, FLT_MAX))
		{
			m_ParticleRender->SetNoiseForceTerm(fTerm);
		}
		AddItemHeight();

		float fScale = m_ParticleRender->GetNoiseForceScale();
		ImGui::Text("Noise Force Scale");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Noise Force Scale", &fScale))
		{
			m_ParticleRender->SetNoiseForceScale(fScale);
		}
		AddItemHeight();

		ImGui::EndDisabled();
		ImGui::TreePop();
	}
	AddItemHeight();
}

void ParticleRenderUI::RenderModule()
{
	// 크기

	if (ImGui::TreeNode("Render"))
	{
		bool bRender = m_ParticleRender->GetModule(PARTICLE_MODULE::RENDER);
		ImGui::Text("Render");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##Render", &bRender))
		{
			m_ParticleRender->SetModule(PARTICLE_MODULE::RENDER, bRender);
		}
		AddItemHeight();

		ImGui::BeginDisabled(!bRender);

		Vector4 fColor = m_ParticleRender->GetEndColor();
		ImGui::Text("End Color");
		ImGui::SameLine(GetTab());
		if (ImGui::ColorEdit4("##End Color", fColor))
		{
			m_ParticleRender->SetEndColor(fColor);
		}
		AddItemHeight();

		bool fFadeOut = m_ParticleRender->GetFadeOut();
		ImGui::Text("Fade Out");
		ImGui::SameLine(GetTab());
		if (ImGui::Checkbox("##Noise Force Scale", &fFadeOut))
		{
			m_ParticleRender->SetFadeOut(fFadeOut);
		}
		AddItemHeight();

		float fFadeOutTime = m_ParticleRender->GetFadeOutStartRatio();
		ImGui::Text("Fade Out Start");
		ImGui::SameLine(GetTab());
		if (ImGui::DragFloat("##Fade Out Start", &fFadeOutTime, 0.01f, 0.f, 1.f))
		{
			m_ParticleRender->SetFadeOutStartRatio(fFadeOutTime);
		}
		AddItemHeight();

		bool bVelocityAlignment = m_ParticleRender->GetVelocityAlignment();
		bool bCrossMesh = m_ParticleRender->GetCrossMesh();
		ImGui::Text("Velocity Alignment");
		ImGui::SameLine(GetTab());
		ImGui::Checkbox("##Velocity Alignment", &bVelocityAlignment);
		ImGui::SameLine();
		ImGui::Text("Cross Mesh");
		ImGui::SameLine();
		ImGui::Checkbox("##Cross Mesh", &bCrossMesh);
		if (bVelocityAlignment != m_ParticleRender->GetVelocityAlignment()
			|| bCrossMesh != m_ParticleRender->GetCrossMesh())
		{
			m_ParticleRender->SetVelocityAlignment(bVelocityAlignment, bCrossMesh);
		}
		AddItemHeight();
		ImGui::EndDisabled();
		ImGui::TreePop();
	}
	AddItemHeight();
}
