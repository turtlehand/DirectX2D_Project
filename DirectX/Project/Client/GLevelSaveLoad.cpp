#include "pch.h"
#include "GLevelSaveLoad.h"

#include <Engine/GLevelManager.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>

#include <Engine/GCollisionManager.h>

#include <Engine/GGameObject.h>
#include <Engine/components.h>
#include <Engine/GComponent.h>

#include <Engine/GScript.h>
#include <Practice/GScriptManager.h>

void GLevelSaveLoad::SaveLevel(wstring _FilePath)
{
	GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	// ������Ʈ ����
	SaveProjectSetting(pFile);

	// ���� �̸�
	SaveWString(pCurLevel->GetName(), pFile);

	// ��� ���̾��� ��� �ֻ��� ������Ʈ�� ����Ѵ�.
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		GLayer* pLayer = pCurLevel->GetLayer(i);
		
		// ���̾� �̸�
		SaveWString(pLayer->GetName(), pFile);

		const vector<GGameObject*>& vecObjects = pLayer->GetParentObjects();

		// ������Ʈ ����
		size_t ObjectSize = vecObjects.size();
		fwrite(&ObjectSize, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			SaveGameObject(vecObjects[j], pFile);
		}
	}

	fclose(pFile);
}

GLevel* GLevelSaveLoad::LoadLevel(wstring _FilePath)
{
	GLevel* pNewLevel = new GLevel;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	assert(pFile);

	// ������Ʈ ����
	LoadProjectSetting(pFile);

	// ���� �̸�
	wstring Name;
	LoadWString(Name, pFile);
	pNewLevel->SetName(Name);

	// ��� ���̾��� ��� �ֻ��� ������Ʈ�� ����Ѵ�.
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		GLayer* pLayer = pNewLevel->GetLayer(i);

		// ���̾� �̸�
		Name = L"";
		LoadWString(Name, pFile);
		pLayer->SetName(Name);

		// ������Ʈ ����
		size_t ObjectSize = 0;
		fread(&ObjectSize, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < ObjectSize; ++j)
		{
			GGameObject* pObject = LoadGameObject(pFile);
			pLayer->AddGameObject(pObject);
		}
	}

	fclose(pFile);

	return pNewLevel;
}

void GLevelSaveLoad::SaveGameObject(GGameObject* _Object, FILE* _File)
{
	// ������Ʈ �̸� ����
	SaveWString(_Object->GetName(), _File);

	int Layer = _Object->GetLayer();
	fwrite(&Layer, sizeof(int), 1, _File);

	// ������Ʈ ���� ����
	COMPONENT_TYPE ComType = COMPONENT_TYPE::END;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		GComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pComponent)
			continue;

		// ������Ʈ Ÿ�� ����
		ComType = pComponent->GetType();
		fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		// �ش� ������Ʈ ���� ����
		pComponent->SaveToFile(_File);
	}

	// ������Ʈ ���� ����
	ComType = COMPONENT_TYPE::END;
	fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

	// ��ũ��Ʈ ���� ����
	vector<GScript*> vecScripts = _Object->GetScripts();
	size_t SSize = vecScripts.size();
	fwrite(&SSize, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < SSize; ++i)
	{
		GScript* pScript = vecScripts[i];

		// ��ũ��Ʈ Ÿ�� ����
		wstring ScriptName = GScriptManager::GetScriptName(pScript);
		SaveWString(ScriptName, _File);

		// �ش� ��ũ��Ʈ ���� ����
		pScript->SaveToFile(_File);
	}

	// �ڽ� ������Ʈ ����
	size_t CSize = _Object->GetChild().size();
	fwrite(&CSize, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < _Object->GetChild().size(); ++i)
	{
		SaveGameObject(_Object->GetChild()[i], _File);
	}
}

GGameObject* GLevelSaveLoad::LoadGameObject(FILE* _File)
{
	// ������Ʈ �̸� �ҷ�����
	wstring Name;
	LoadWString(Name, _File);

	int Layer;
	fread(&Layer, sizeof(int), 1, _File);

	GGameObject* pNewObject = new GGameObject(Layer);
	pNewObject->SetName(Name);

	// ������Ʈ ���� �ҷ�����
	COMPONENT_TYPE ComType = COMPONENT_TYPE::END;

	while (true)
	{
		// ������Ʈ Ÿ�� �ҷ�����
		fread(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		// ���� �� Ÿ���̶�� ������Ʈ �ҷ����� ����
		if (ComType == COMPONENT_TYPE::END)
			break;
		
		GComponent* pComponent = nullptr;

		switch (ComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new GTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new GCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//pComponent = new CCollider3D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new GLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			//pComponent = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			pComponent = new GCamera;
			break;
		case COMPONENT_TYPE::RIGIDBODY2D:
			pComponent = new GRigidBody2D;
			break;
		case COMPONENT_TYPE::BOUNDINGBOX:
			//pComponent = new CBoundingBox;
			break;
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
		case COMPONENT_TYPE::SKYBOX:
			//pComponent = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:
			//pComponent = new CDecal;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			//pComponent = new CLandScape;
			break;
		}

		pNewObject->AddComponent(pComponent);

		pComponent->LoadFromFile(_File);

	}

	// ��ũ��Ʈ ���� �ҷ�����
	size_t SSize;
	fread(&SSize, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < SSize; ++i)
	{
		GScript* pScript = nullptr;

		// ��ũ��Ʈ Ÿ�� �ҷ�����
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// ��ũ��Ʈ Ÿ������ ��ũ��Ʈ�� �������Ѽ� ������Ʈ�� �־���
		pScript = GScriptManager::GetScript(ScriptName);
		pNewObject->AddComponent(pScript);

		// ��ũ��Ʈ�� ������ �����͸� �ٽ� ������
		pScript->LoadFromFile(_File);
	}

	// �ڽ� ������Ʈ �ҷ�����
	size_t CSize;
	fread(&CSize, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < CSize; ++i)
	{
		pNewObject->AddChild(LoadGameObject(_File));
	}

	return pNewObject;
}

void GLevelSaveLoad::SaveProjectSetting(FILE* _File)
{

	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(0)->SetName(L"Default");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(1)->SetName(L"Background");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(2)->SetName(L"Platform");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(3)->SetName(L"Player");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(4)->SetName(L"Monster");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(5)->SetName(L"NPC");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(6)->SetName(L"Object");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(7)->SetName(L"Item");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(8)->SetName(L"Attack");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(9)->SetName(L"Player_Attack");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(10)->SetName(L"Enemy_Attack");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(11)->SetName(L"Ending_Trigger");
	GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(12)->SetName(L"Platfrom_Checker");

	// ==============
	// ���̾�
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		wstring LayerName = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(i)->GetName();
		SaveWString(LayerName, _File);
	}

	GCollisionManager::GetInst()->ResetCollisionLayer();
	//GCollisionManager::GetInst()->CollisionLayerCheck(0, 0);	// �⺻ �⺻
	GCollisionManager::GetInst()->CollisionLayerCheck(2, 3);	// �÷��� �÷��̾�
	GCollisionManager::GetInst()->CollisionLayerCheck(2, 4);	// �÷��� ����
	//GCollisionManager::GetInst()->CollisionLayerCheck(2, 5);	// �÷��� NPC
	//GCollisionManager::GetInst()->CollisionLayerCheck(2, 6);	// �÷��� ��Ÿ
	//GCollisionManager::GetInst()->CollisionLayerCheck(2, 7);	// �÷��� ������
	GCollisionManager::GetInst()->CollisionLayerCheck(2, 12);	// �÷��� platform üĿ
	 
	//GCollisionManager::GetInst()->CollisionLayerCheck(3, 4);	// �÷��̾� ����
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 5);	// �÷��� NPC
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 6);	// �÷��̾� ��Ÿ
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 7);	// �÷��̾� ������
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 10);	// �÷��̾� ���
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 11);	// �÷��̾� ���� Ʈ����
	
	GCollisionManager::GetInst()->CollisionLayerCheck(4, 9);	// �� �ð�
	 
	GCollisionManager::GetInst()->CollisionLayerCheck(5, 9);	// NPC, �ð�
	 
	GCollisionManager::GetInst()->CollisionLayerCheck(6, 9);	// ��Ÿ, �ð�
	

	// ============
	// �浹
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		UINT LayerCheck = GCollisionManager::GetInst()->GetCollisionLayer(i);
		fwrite(&LayerCheck, sizeof(UINT), 1, _File);
	}
	
}

void GLevelSaveLoad::LoadProjectSetting(FILE* _File)
{
	// ==============
	// ���̾�

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		wstring LayerName;
		LoadWString(LayerName, _File);
		GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(i)->SetName(LayerName);
	}

	// ============
	// �浹
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		UINT LayerCheck;
		fread(&LayerCheck, sizeof(UINT), 1, _File);
		GCollisionManager::GetInst()->SetCollisionLayer(i, LayerCheck);
	}
}
