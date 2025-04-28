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

	// 프로젝트 세팅
	SaveProjectSetting(pFile);

	// 레벨 이름
	SaveWString(pCurLevel->GetName(), pFile);

	// 모든 레이어의 모든 최상위 오브젝트를 기록한다.
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		GLayer* pLayer = pCurLevel->GetLayer(i);
		
		// 레이어 이름
		SaveWString(pLayer->GetName(), pFile);

		const vector<GGameObject*>& vecObjects = pLayer->GetParentObjects();

		// 오브젝트 개수
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

	// 프로젝트 세팅
	LoadProjectSetting(pFile);

	// 레벨 이름
	wstring Name;
	LoadWString(Name, pFile);
	pNewLevel->SetName(Name);

	// 모든 레이어의 모든 최상위 오브젝트를 기록한다.
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		GLayer* pLayer = pNewLevel->GetLayer(i);

		// 레이어 이름
		Name = L"";
		LoadWString(Name, pFile);
		pLayer->SetName(Name);

		// 오브젝트 개수
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
	// 오브젝트 이름 저장
	SaveWString(_Object->GetName(), _File);

	int Layer = _Object->GetLayer();
	fwrite(&Layer, sizeof(int), 1, _File);

	// 컴포넌트 정보 저장
	COMPONENT_TYPE ComType = COMPONENT_TYPE::END;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		GComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pComponent)
			continue;

		// 컴포넌트 타입 저장
		ComType = pComponent->GetType();
		fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		// 해당 컴포넌트 정보 저장
		pComponent->SaveToFile(_File);
	}

	// 컴포넌트 정보 마감
	ComType = COMPONENT_TYPE::END;
	fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

	// 스크립트 정보 저장
	vector<GScript*> vecScripts = _Object->GetScripts();
	size_t SSize = vecScripts.size();
	fwrite(&SSize, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < SSize; ++i)
	{
		GScript* pScript = vecScripts[i];

		// 스크립트 타입 저장
		wstring ScriptName = GScriptManager::GetScriptName(pScript);
		SaveWString(ScriptName, _File);

		// 해당 스크립트 정보 저장
		pScript->SaveToFile(_File);
	}

	// 자식 오브젝트 저장
	size_t CSize = _Object->GetChild().size();
	fwrite(&CSize, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < _Object->GetChild().size(); ++i)
	{
		SaveGameObject(_Object->GetChild()[i], _File);
	}
}

GGameObject* GLevelSaveLoad::LoadGameObject(FILE* _File)
{
	// 오브젝트 이름 불러오기
	wstring Name;
	LoadWString(Name, _File);

	int Layer;
	fread(&Layer, sizeof(int), 1, _File);

	GGameObject* pNewObject = new GGameObject(Layer);
	pNewObject->SetName(Name);

	// 컴포넌트 정보 불러오기
	COMPONENT_TYPE ComType = COMPONENT_TYPE::END;

	while (true)
	{
		// 컴포넌트 타입 불러오기
		fread(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		// 만약 끝 타입이라면 컴포넌트 불러오기 종료
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

	// 스크립트 정보 불러오기
	size_t SSize;
	fread(&SSize, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < SSize; ++i)
	{
		GScript* pScript = nullptr;

		// 스크립트 타입 불러오기
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// 스크립트 타입으로 스크립트를 생성시켜서 오브젝트에 넣어줌
		pScript = GScriptManager::GetScript(ScriptName);
		pNewObject->AddComponent(pScript);

		// 스크립트가 저장한 데이터를 다시 복구함
		pScript->LoadFromFile(_File);
	}

	// 자식 오브젝트 불러오기
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
	// 레이어
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		wstring LayerName = GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(i)->GetName();
		SaveWString(LayerName, _File);
	}

	GCollisionManager::GetInst()->ResetCollisionLayer();
	//GCollisionManager::GetInst()->CollisionLayerCheck(0, 0);	// 기본 기본
	GCollisionManager::GetInst()->CollisionLayerCheck(2, 3);	// 플랫폼 플레이어
	GCollisionManager::GetInst()->CollisionLayerCheck(2, 4);	// 플랫폼 몬스터
	//GCollisionManager::GetInst()->CollisionLayerCheck(2, 5);	// 플랫폼 NPC
	//GCollisionManager::GetInst()->CollisionLayerCheck(2, 6);	// 플랫폼 기타
	//GCollisionManager::GetInst()->CollisionLayerCheck(2, 7);	// 플랫폼 아이템
	GCollisionManager::GetInst()->CollisionLayerCheck(2, 12);	// 플랫폼 platform 체커
	 
	//GCollisionManager::GetInst()->CollisionLayerCheck(3, 4);	// 플레이어 몬스터
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 5);	// 플랫폼 NPC
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 6);	// 플레이어 기타
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 7);	// 플레이어 아이템
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 10);	// 플레이어 몬공
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 11);	// 플레이어 엔딩 트리거
	
	GCollisionManager::GetInst()->CollisionLayerCheck(4, 9);	// 몬 플공
	 
	GCollisionManager::GetInst()->CollisionLayerCheck(5, 9);	// NPC, 플공
	 
	GCollisionManager::GetInst()->CollisionLayerCheck(6, 9);	// 기타, 플공
	

	// ============
	// 충돌
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		UINT LayerCheck = GCollisionManager::GetInst()->GetCollisionLayer(i);
		fwrite(&LayerCheck, sizeof(UINT), 1, _File);
	}
	
}

void GLevelSaveLoad::LoadProjectSetting(FILE* _File)
{
	// ==============
	// 레이어

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		wstring LayerName;
		LoadWString(LayerName, _File);
		GLevelManager::GetInst()->GetCurrentLevel()->GetLayer(i)->SetName(LayerName);
	}

	// ============
	// 충돌
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		UINT LayerCheck;
		fread(&LayerCheck, sizeof(UINT), 1, _File);
		GCollisionManager::GetInst()->SetCollisionLayer(i, LayerCheck);
	}
}
