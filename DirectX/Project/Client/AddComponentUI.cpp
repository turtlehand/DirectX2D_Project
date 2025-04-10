#include "pch.h"
#include "AddComponentUI.h"

#include "ImGuiManager.h"
#include "ListUI.h"
#include "InspectorUI.h"

#include <Engine/GAssetManager.h>
#include <Engine/components.h>
#include <Practice/GScriptManager.h>

AddComponentUI::AddComponentUI()
	:m_Target(nullptr)
{
	SetName("AddComponent");
}

AddComponentUI::~AddComponentUI()
{
}

void AddComponentUI::SetTarget(GGameObject* _Target)
{
	m_Target = _Target;
	// m_Target이 nullptr이거나, ComponentUI가 담당하는 Component를 GameObjct가 가지고 있지 않는다면
	if (nullptr == m_Target)
		SetActive(false);
	else
		SetActive(true);
}

void AddComponentUI::SelectComponent(DWORD_PTR _Data)
{
	ListUI* pList = (ListUI*)_Data;
	string ComName = pList->GetString();
	bool isValid = false;

	// 컴포넌트
	GComponent* pComponent = nullptr;
	COMPONENT_TYPE ComType = COMPONENT_TYPE::END;
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (ComName == COMPONENT_TYPE_STR[i])
		{
			ComType = (COMPONENT_TYPE)i;
			isValid = true;
		}
	}

	if (isValid)
	{
		switch (ComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new GTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new GCollider2D;
			break;
		//case COMPONENT_TYPE::COLLIDER3D:
			//pComponent = new CCollider3D;
			//break;
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new GLight2D;
			break;
		//case COMPONENT_TYPE::LIGHT3D:
			////pComponent = new CLight3D;
			//break;
		case COMPONENT_TYPE::CAMERA:
			pComponent = new GCamera;
			break;
		case COMPONENT_TYPE::RIGIDBODY2D:
			pComponent = new GRigidBody2D;
			break;
		//case COMPONENT_TYPE::BOUNDINGBOX:
			//pComponent = new CBoundingBox;
			//break;
		case COMPONENT_TYPE::MESH_RENDER:
			pComponent = new GMeshRender;
			break;
		case COMPONENT_TYPE::SPRITE_RENDER:
			pComponent = new GSpriteRender;
			break;
		case COMPONENT_TYPE::FLIPBOOK_RENDER:
			pComponent = new GFlipbookRender;
			break;
		case COMPONENT_TYPE::TILE_RENDER:
			pComponent = new GTileRender;
			break;
		case COMPONENT_TYPE::PARTICLE_RENDER:
			pComponent = new GParticleRender;
			break;
		//case COMPONENT_TYPE::SKYBOX:
			//pComponent = new CSkyBox;
			//break;
		//case COMPONENT_TYPE::DECAL:
			//pComponent = new CDecal;
			//break;
		//case COMPONENT_TYPE::LANDSCAPE:
			//pComponent = new CLandScape;
			//break;
		}

		if (pComponent == nullptr)
		{
			MessageBox(nullptr, ToWString(ComName).c_str(), L"존재하지 않는 컴포넌트입니다.", MB_OK);
			return ;
		}

		m_Target->AddComponent(pComponent);
	}

	// 스크립트라면
	GScript* pScript = GScriptManager::GetScript(ToWString(ComName));

	if (pScript != nullptr)
	{
		isValid = true;
		m_Target->AddComponent(pScript);
	}

	assert(isValid);

	InspectorUI* pInspector = ImGuiManager::GetInst()->FindUI<InspectorUI>("InspectorUI");
	pInspector->SetTargetObject(m_Target);

}

void AddComponentUI::Render_UI()
{
	if (!IsValid(m_Target))
	{
		SetActive(false);
		return;
	}

	if (ImGui::Button("AddComponent"))
	{
		ListUI* pList = ImGuiManager::GetInst()->FindUI<ListUI>("ListUI");
		assert(pList);

		pList->SetActive(true);

		vector<string> Items;
		vector<wstring> vecScriptName;
		GScriptManager::GetScriptInfo(vecScriptName);

		for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
		{
			// 이미 있는 컴포넌트는 제외
			if (m_Target->GetComponent((COMPONENT_TYPE)i))
				continue;
			// 렌더 컴포넌트가 있을 때는 Render 컴포넌트 제외
			if (m_Target->Renderer() != nullptr)
			{
				if ((int)COMPONENT_TYPE::RIGIDBODY2D < i && i < (int)COMPONENT_TYPE::END)
					continue;
			}
			Items.push_back(COMPONENT_TYPE_STR[i]);
		}
		for (int i = 0; i < vecScriptName.size(); ++i)
		{
			Items.push_back(ToString(vecScriptName[i]));
		}

		pList->SetItems(Items);

		pList->AddDelegate(this, (EUI_DELEGATE_1)&AddComponentUI::SelectComponent);
	}
}