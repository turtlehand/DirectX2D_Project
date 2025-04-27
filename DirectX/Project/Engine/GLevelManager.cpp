#include "pch.h"
#include "GLevelManager.h"

#include "GDevice.h"
#include "GLevel.h"
#include "GLayer.h"

#include "GGameObject.h"
#include "assets.h"

#include "GRenderManager.h"
#include "GTaskManager.h"

LEVEL_LOAD  GLevelManager::g_Load_Level = nullptr;

GLevelManager::GLevelManager()
	: m_CurLevel(nullptr)
{
	m_LayerName = new char* [MAX_LAYER];
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_LayerName[i] = new char[255];
	}
}

GLevelManager::~GLevelManager()
{
	delete m_CurLevel;

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		delete[] m_LayerName[i];
	}
	delete[] m_LayerName;

}

LEVEL_STATE GLevelManager::GetCurrentLevelState()
{
	return m_CurLevel->GetState();
}

const char** GLevelManager::GetLayerNames()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		string LayerText = to_string(i) + " : " + ToString(GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(i)->GetName());
		strcpy_s(m_LayerName[i], 255, LayerText.c_str());
	}

	return const_cast<const char**>(m_LayerName);
}

const char* GLevelManager::GetLayerName(int _Layer)
{
	string LayerText = to_string(_Layer) + " : " + ToString(GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(_Layer)->GetName());
	strcpy_s(m_LayerName[_Layer], 255, LayerText.c_str());

	return const_cast<const char*>(m_LayerName[_Layer]);
}

void GLevelManager::LoadLevel(const wstring& _RelativePath)
{
	assert(m_CurLevel->GetState() == LEVEL_STATE::PLAY);

	wstring FullPath = GPathManager::GetContentPath() + _RelativePath;

	GLevel* pNextLevel = g_Load_Level(FullPath);

	// 레벨 변경
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param0 = (DWORD_PTR)pNextLevel;
	GTaskManager::GetInst()->AddTask(task);

	// 레벨 상태를 Play로 변경
	task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	task.Param0 = (DWORD_PTR)LEVEL_STATE::PLAY;
	GTaskManager::GetInst()->AddTask(task);
}

void GLevelManager::ChangeLevel(GLevel* _NextLevel)
{
	assert(m_CurLevel != _NextLevel);
	delete m_CurLevel;
	m_CurLevel = _NextLevel;
	// 레벨이 변경되면서, RenderManager가 들고 있던 삭제된 레벨의 카메라를 전부 해제시킨다.
	GRenderManager::GetInst()->ClearCamera();
}

void GLevelManager::Init()
{
	m_CurLevel = new GLevel;
}

void GLevelManager::Progress()
{
	// =====
	// Tick
	// =====
	m_CurLevel->Update();

	m_CurLevel->Deregister();

	m_CurLevel->FinalUpdate();
}