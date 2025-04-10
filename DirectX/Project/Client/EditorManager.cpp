#include "pch.h"
#include "EditorManager.h"

#include <Engine/GRenderManager.h>
#include <Engine/components.h>

#include "GGameObjectEX.h"
#include "EditorCameraScript.h"

EditorManager::EditorManager()
{

}

EditorManager::~EditorManager()
{
	Delete_Vec(m_vecEditorObject);
}

void EditorManager::Init()
{
	// Editor 용 카메라 제작
	m_EditorCamera = new GGameObjectEX();
	m_EditorCamera->AddComponent<GTransform>();
	m_EditorCamera->AddComponent<GCamera>();
	m_EditorCamera->AddComponent<EditorCameraScript>();

	m_EditorCamera->SetName(L"Editor Camera");

	m_EditorCamera->Transform()->SetRelativePos(Vector3(0.f, 0.f, -100.f));

	m_EditorCamera->Camera()->CheckLayerAll();
	m_EditorCamera->Camera()->SetProjType(PROJECTION_TYPE::ORTHOGRAPHIC);
	m_EditorCamera->Camera()->SetFar(100000.f);

	m_vecEditorObject.push_back(m_EditorCamera);

	// RenderManager에 Editor 카메라 등록
	GRenderManager::GetInst()->RegisterEditorCamera(m_EditorCamera->Camera());

}

void EditorManager::Progress()
{
	for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
	{
		m_vecEditorObject[i]->Update();
	}

	for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
	{
		m_vecEditorObject[i]->FinalUpdate();
	}
}
