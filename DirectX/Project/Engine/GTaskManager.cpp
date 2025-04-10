#include "pch.h"
#include "GTaskManager.h"

#include "GGameObject.h"
#include "GComponent.h"

#include "GLevelManager.h"
#include "GLevel.h"

#include "GAssetManager.h"
#include "GAsset.h"



GTaskManager::GTaskManager()
	: m_vecTask{}
	, m_vecGC{}
	, m_LevelChanged(true)
{

}

GTaskManager::~GTaskManager()
{

}

void GTaskManager::Progress()
{
	m_LevelChanged = !m_vecGC.empty();

	for (size_t i = 0;i < m_vecGC.size();++i)
	{
		delete m_vecGC[i];
	}
	m_vecGC.clear();

	
	for (size_t i = 0;i < m_vecTask.size();++i)
	{
		const tTask& task = m_vecTask[i];
		GLevel* pLevel = GLevelManager::GetInst()->GetCurrentLevel();
		
		switch (task.Type)
		{
		case TASK_TYPE::DESTROY_OBJECT:
		{
			GGameObject* pObject = (GGameObject*)task.Param0;

			if (!pObject->m_Dead)
			{
				pObject->m_Dead = true;
				m_vecGC.push_back(pObject);

				pObject->End();

			}
			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::CREATE_OBJECT:
		{
			GGameObject* pNew = (GGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;
			pLevel->AddGameObject(pNew, LayerIdx);

			if(GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::STOP)
				pNew->Begin();

			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::CHANGE_OBJECT_LAYER:
		{

			GGameObject* pObject = (GGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;
			pObject->m_Layer = LayerIdx;
			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::ADD_CHILD:
		{
			GGameObject* pParent = (GGameObject*)task.Param0;
			GGameObject* pChild = (GGameObject*)task.Param1;

			// 1. �θ� ���� �Ѵٸ�
			if (pChild->GetParent() != nullptr)
			{
				// �θ�� �����Ѵ�.
				pChild->DisconnectWithParent();

				// ���ο� �θ�� �������� �ʴ´ٸ� ���̾ ����Ѵ�.
				if (pParent == nullptr)
					pChild->RegisterAsParent();
				else
					pParent->AddChild(pChild);
			}

			// 2. �θ� �������� �ʴٸ�
			else
			{
				// ���ο� �θ�� �������� �ʴ´ٸ� �����Ѵ�.
				if (pParent == nullptr)
					break;
				pChild->DisconnectWithLayerAsParent();
				pParent->AddChild(pChild);
			}

			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::DESTROY_COMPONENT:
		{

			GGameObject* pObject = (GGameObject*)task.Param0;
			GComponent* pComponent = (GComponent*)task.Param1;

			// ������Ʈ�� �̹� Dead�ų� ������Ʈ�� Dead ���
			if (!pObject->m_Dead && !pComponent->m_Dead)
			{
				pComponent->m_Dead = true;
				m_vecGC.push_back(pComponent);

				// Stop ���°� �ƴ϶�� OnDestory�� ȣ�����ش�.
				if (GLevelManager::GetInst()->GetCurrentLevelState() != LEVEL_STATE::STOP)
					pComponent->End();

			}
			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)task.Param0;
			GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();
			pCurLevel->ChangeState(NextState);
		}
		break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			GLevel* pNextLevel = (GLevel*)task.Param0;
			GLevelManager::GetInst()->ChangeLevel(pNextLevel);

			m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::DELETE_ASSET:
		{
			GAsset* pAsset = (GAsset*)task.Param0;
			GAssetManager::GetInst()->DeleteAsset(pAsset);
		}
		break;

		default:
			break;
		}
	}
	m_vecTask.clear();

}
