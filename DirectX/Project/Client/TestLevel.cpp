#include "pch.h"

#include <Engine/GLevelManager.h>
#include <Engine/GDevice.h>
#include <Engine/GLevel.h>
#include <Engine/GLayer.h>
#include <Engine/GAssetManager.h>
#include <Engine/GCollisionManager.h>
#include <Engine/GGameObject.h>
#include <Engine/components.h>
#include <Engine/assets.h>
#include <Engine/GTaskManager.h>

#include "GLevelSaveLoad.h"

void CreateTestLevel()
{
	
	//Ptr<GSound> pSound = GAssetManager::GetInst()->FindAsset<GSound>(L"Sound\\BGM_Stage1.
	// ");
	//pSound->Play(0, 0.2f, false);

#ifndef GAME_RELEASED
	GLevel* pCurLevel = GLevelManager::GetInst()->GetCurrentLevel();

	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Background");
	pCurLevel->GetLayer(2)->SetName(L"Platform");
	pCurLevel->GetLayer(3)->SetName(L"Player");
	pCurLevel->GetLayer(4)->SetName(L"Monster");
	pCurLevel->GetLayer(5)->SetName(L"NPC");
	pCurLevel->GetLayer(6)->SetName(L"Object");
	pCurLevel->GetLayer(7)->SetName(L"Item");
	pCurLevel->GetLayer(8)->SetName(L"Attack");
	pCurLevel->GetLayer(9)->SetName(L"Player_Attack");
	pCurLevel->GetLayer(10)->SetName(L"Enemy_Attack");
	pCurLevel->GetLayer(11)->SetName(L"Ending_Trigger");

	pCurLevel->GetLayer(31)->SetName(L"UI");

	
	// 카메라 역할 오브젝트 생성
	GGameObject* pCamObj = new GGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new GTransform);
	pCamObj->AddComponent(new GCamera);
	//pCamObj->AddComponent(new GCameraMove);

	pCamObj->Transform()->SetRelativePos(0.f, 0.f, -100.f);

	pCamObj->Camera()->SetProjType(PROJECTION_TYPE::ORTHOGRAPHIC);
	//pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetOrthoScaleX(pCamObj->Camera()->GetOrthoScaleX() / 10);
	pCamObj->Camera()->SetPriority(0); // 메인 카메라로 설정
	pCamObj->Camera()->CheckLayerAll();
	pCamObj->Camera()->CheckLayer(31);

	SpawnGameObject(pCamObj);
	
	// 광원 설치
	GGameObject* pLight = new GGameObject;
	pLight->SetName(L"Light");

	pLight->AddComponent(new GTransform);
	pLight->AddComponent(new GLight2D);

	pLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetLightColor(Vector3(1.f, 1.f, 1.f));

	SpawnGameObject(pLight);

	GCollisionManager::GetInst()->CollisionLayerCheck(0, 0);

#else

	wstring FilePath = GPathManager::GetContentPath();
	FilePath += L"Level\\Map.lv";

	GLevel* pNextLevel = GLevelSaveLoad::LoadLevel(FilePath);

	// 레벨 변경
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param0 = (DWORD_PTR)pNextLevel;

	GTaskManager::GetInst()->AddTask(task);
	
	task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	task.Param0 = (DWORD_PTR)LEVEL_STATE::PLAY;
	GTaskManager::GetInst()->AddTask(task);

#endif

	/*
	pLight = new GGameObject;
	pLight->SetName(L"Light");

	pLight->AddComponent(new GTransform);
	pLight->AddComponent(new GLight2D);

	pLight->Transform()->SetRelativePos(200.f, 0.f, 0.f);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLight->Light2D()->SetLightColor(Vector3(0.2f, 0.8f, 0.3));
	pLight->Light2D()->SetRadius(300.f);

	SpawnGameObject(pLight, 0);


	// Parent Object
	GGameObject* pObject = new GGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new GTransform);
	//pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new GBoxCollider2D);
	pObject->AddComponent(new GFlipbookRender);
	//pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	//pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetIndependentScale(true);
	pObject->Collider2D()->SetScale(Vec2(110.f, 110.f));
	pObject->Collider2D()->SetOffset(Vec2(0.f, 0.f));

	pObject->FlipbookRender()->AddFlipbook(0, GAssetManager::GetInst()->Load<GFlipbook>(L"Flipbook\\LinkFlipbook.flip", L"Flipbook\\LinkFlipbook.flip"));
	pObject->FlipbookRender()->Play(0, 20, true);


	// Child Object
	GGameObject* pChild = new GGameObject;
	pChild->SetName(L"Child");
	pChild->AddComponent(new GTransform);
	pChild->AddComponent(new GMeshRender);
	pChild->AddComponent(new GBoxCollider2D);

	pChild->Transform()->SetIndependentScale(true);
	pChild->Transform()->SetRelativePos(150.f, 0.f, 0.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pChild->Collider2D()->SetNotifyParent(true);
	pChild->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pChild->Collider2D()->SetScale(Vec2(1.1f, 1.1f));

	pChild->MeshRender()->SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"CircleMesh"));
	pChild->MeshRender()->SetMaterial(GAssetManager::GetInst()->FindAsset<GMaterial>(L"Std2DMtrl"));

	// 부모 자식 연결
	pObject->AddChild(pChild);

	// 레벨에 부모 오브젝트만 추가
	SpawnGameObject(pObject, 3);

	// ========
	// Particle
	// ========
	pObject = new GGameObject;
	pObject->SetName(L"Particle");
	pObject->AddComponent(new GTransform);
	pObject->AddComponent(new GParticleRender);

	pObject->Transform()->SetRelativePos(-500.f, 300.f, 50.f);
	pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	GParticleRender* pRender = pObject->ParticleRender();

	// 파티클 용 텍스쳐 지정
	Ptr<GTexture> pParticleTex = GAssetManager::GetInst()->Load<GTexture>(L"Texture\\particle\\ray.png");
	pRender->SetParticleTex(pParticleTex);

	pRender->SetSpawnRate(20.f);
	pRender->SetSpawnShape(0);
	pRender->SetSpawnShapeScale(Vector3(500.f, 500.f, 500.f));
	pRender->SetMinLife(1.f);
	pRender->SetMaxLife(3.f);
	pRender->SetMinScale(Vector3(100.f, 30.f, 10.f));
	pRender->SetMaxScale(Vector3(200.f, 60.f, 20.f));

	pRender->SetSpaceType(0);

	// AddVelocity 기능 On
	pRender->SetModlue(PARTICLE_MODULE::ADD_VELOCITY, true);
	pRender->SetAddVelocityType(3);
	pRender->SetMinSpeed(100.f);
	pRender->SetMaxSpeed(500.f);
	pRender->SetFixedVelocity(Vector3(0.f, -1.f, 0.f));

	// SpawnBurst 기능
	pRender->SetModlue(PARTICLE_MODULE::SPAWN_BURST, true);
	pRender->SetBurstParticleCount(100);
	pRender->SetBurstRepeatCount(10);
	pRender->SetBurstTerm(2.f);

	// Scale 
	pRender->SetModlue(PARTICLE_MODULE::SCALE, true);
	pRender->SetStartScale(2.f);
	pRender->SetEndScale(2.f);

	// Drag
	//pRender->SetModlue(PARTICLE_MODULE::DRAG, true);
	//pRender->SetDragDestNormalizeAge(0.6f);  // 수명 끝
	//pRender->SetDragLimitSpeed(100.f);		// 속력 0

	// NoiseForce
	pRender->SetModlue(PARTICLE_MODULE::NOISE_FORCE, true);
	pRender->SetNoiseForceTerm(0.2f);
	pRender->SetNoiseForceScale(300.f);

	// Render
	pRender->SetModlue(PARTICLE_MODULE::RENDER, true);
	pRender->SetStartColor(Vector4(1.f, 0.f, 0.f, 1.f));
	pRender->SetEndColor(Vector4(0.f, 0.f, 1.f, 1.f));

	pRender->SetFadeOut(true);
	pRender->SetFadOutStartRatio(0.8f);

	pRender->SetVelocityAlignment(true, true);

	// 오브젝트를 0번 레이어에 추가
	SpawnGameObject(pObject, 0);


	// TileRender Object
	pObject = new GGameObject;
	pObject->SetName(L"TileRender");
	pObject->AddComponent(new GTransform);
	pObject->AddComponent(new GTileRender);

	pObject->Transform()->SetRelativePos(-200.f, 300.f, 500.f);
	pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	pObject->TileRender()->SetRowCol(10, 10);
	pObject->TileRender()->SetTileSize(Vec2(64.f, 64.f));

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			pObject->TileRender()->SetSprite(i, j, GAssetManager::GetInst()->FindAsset<GSprite>(L"Sprite\\TileSprite_0.sprite"));
		}
	}
	SpawnGameObject(pObject, 0);

	// 충돌 레이어 지정
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 5);
	GCollisionManager::GetInst()->CollisionLayerCheck(4, 5);
	GCollisionManager::GetInst()->CollisionLayerCheck(3, 6);
	*/

	/*
	//return;
	// Level 생성
	GLevel* pLevel = GLevelManager::GetInst()->GetCurrentLevel();

	Ptr<GSound> pSound = GAssetManager::GetInst()->FindAsset<GSound>(L"Sound\\BGM_Stage1.wav");
	pSound->Play(0, 0.2f, false);

	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Tile");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(5)->SetName(L"Enermy");
	pLevel->GetLayer(6)->SetName(L"EnermyProjectile");


	// 카메라 역할 오브젝트 생성
	GGameObject* pCamObj = new GGameObject;
	pCamObj->SetName(L"Camera");
	pCamObj->AddComponent<GTransform>();
	pCamObj->AddComponent<GCamera>();
	pCamObj->AddComponent<CameraMove>();

	pCamObj->Transform()->SetRelativePos(0.f, 0.f, -100.f);

	GCamera* pCamComp = (GCamera*)pCamObj->GetComponent(COMPONENT_TYPE::CAMERA);
	pCamComp->SetProjType(PROJECTION_TYPE::PERSPECTIVE);
	pCamComp->SetPriority(0);	// 메인 카메라로 설정
	pCamComp->CheckLayerAll();
	pCamComp->CheckLayer(31);

	SpawnGameObject(pCamObj, 0);

	GGameObject* pObject = nullptr;


	// Level 에 넣을 GameObject 생성
	pObject = new GGameObject;
	pObject->SetName(L"Light");
	pObject->AddComponent<GTransform>();
	pObject->AddComponent<GLight2D>();

	pObject->Transform()->SetRelativePos(Vector3(0.f, 0.f, 0.f));

	pObject->GetComponent<GLight2D>()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->GetComponent<GLight2D>()->SetLightColor(Vector3(0.5f, 0.5f, 0.5f));

	// Level 에 넣을 GameObject 생성
	GGameObject* pChildObject = new GGameObject;
	pChildObject->SetName(L"Light2");
	pChildObject->AddComponent<GTransform>();
	pChildObject->AddComponent<GLight2D>();
	//pChildObject->AddComponent<Player>();

	pChildObject->Transform()->SetRelativePos(Vector3(-200.f, 0.f, 0.f));

	pChildObject->GetComponent<GLight2D>()->SetLightType(LIGHT_TYPE::SPOT);
	pChildObject->GetComponent<GLight2D>()->SetLightColor(Vector3(1.f, 1.f, 1.f));
	pChildObject->GetComponent<GLight2D>()->SetRadius(300.f);
	pChildObject->GetComponent<GLight2D>()->SetDegree(30.f);
	pObject->AddChild(pChildObject);
	// 오브젝트를 0번 레이어에 추가
	SpawnGameObject(pObject, 0);

	pObject = new GGameObject;
	pObject->SetName(L"TileRender");
	pObject->AddComponent<GTransform>();
	pObject->AddComponent<GTileRender>();

	pObject->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	pObject->GetComponent<GTileRender>()->SetRowCol(10, 10);
	pObject->GetComponent<GTileRender>()->SetTileSize(Vector2(64.f, 64.f));

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			pObject->GetComponent<GTileRender>()->SetSprite(i, j, GAssetManager::GetInst()->Load<GSprite>(L"Sprite\\TileSprite_0.sprite", L"Sprite\\TileSprite_0.sprite"));
		}
	}
	SpawnGameObject(pObject, 1);

	// Level 에 넣을 GameObject 생성
	pObject = new GGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent<GTransform>();
	pObject->AddComponent<GMeshRender>();
	pObject->AddComponent<Player>();

	pObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	pObject->GetComponent<GMeshRender>()->SetMesh(GAssetManager::GetInst()->FindAsset<GMesh>(L"RectMesh"));
	pObject->GetComponent<GMeshRender>()->SetMaterial(GAssetManager::GetInst()->FindAsset<GMaterial>(L"Std2DMtrl"));

	pObject->Renderer()->GetMaterial()->SetTexture(TEX_PARAM::TEX_1, GAssetManager::GetInst()->FindAsset<GTexture>(L"Texture\\noise\\noise_03.jpg"));

	// 오브젝트를 0번 레이어에 추가
	SpawnGameObject(pObject, 3);



	// 파티클 렌더

	pObject = new GGameObject;
	pObject->SetName(L"Particle");
	pObject->AddComponent<GTransform>();
	pObject->AddComponent<GParticleRender>();

	GParticleRender* pPRender = pObject->Renderer<GParticleRender>();

	pObject->Transform()->SetRelativePos(0.f, 500.f, 0.f);

	Ptr<GTexture> pParticleTex = GAssetManager::GetInst()->Load<GTexture>(L"Texture\\particle\\ray.png", L"Texture\\particle\\ray.png");
	pPRender->SetParticleTex(pParticleTex);
	pPRender->SetSpawnRate(10.f);
	pPRender->SetSpawnShape(0);
	pPRender->SetSpawnShapeScale(Vector3(500.f, 100.f, 100.f));
	pPRender->SetMinLife(1.f);
	pPRender->SetMaxLife(5.f);
	pPRender->SetSpaceType(1);
	pPRender->SetMaxScale(Vector3(300.f, 300.f, 300.f));
	pPRender->SetMinScale(Vector3(300.f, 300.f, 300.f));

	pPRender->SetModule(PARTICLE_MODULE::ADD_VELOCITY, true);
	pPRender->SetAddVelocityType(3);
	pPRender->SetMinSpeed(10);
	pPRender->SetMaxSpeed(100);
	pPRender->SetFixedVelocity(Vector3(0.f, -1.f, 0.f));



	// Render
	pPRender->SetModlue(PARTICLE_MODULE::RENDER, true);
	pPRender->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
	pPRender->SetEndColor(Vector4(1.f, 1.f, 1.f, 1.f));

	pPRender->SetFadeOut(true);
	pPRender->SetFadOutStartRatio(0.0f);

	pPRender->SetVelocityAlignment(true, true);


	// 오브젝트를 0번 레이어에 추가
	SpawnGameObject(pObject, 0);




	*/
}