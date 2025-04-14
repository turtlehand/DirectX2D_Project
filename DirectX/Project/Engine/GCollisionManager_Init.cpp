#include "pch.h"
#include "GCollisionManager.h"

//PxDefaultAllocator		gAllocator;
//PxDefaultErrorCallback	gErrorCallback;

//PxFoundation* gFoundation = NULL;
//PxPhysics* gPhysics = NULL;

//PxDefaultCpuDispatcher* gDispatcher = NULL;
//PxScene* gScene = NULL;

//PxMaterial* gMaterial = NULL;

//PxPvd* gPvd = NULL;

//PxReal stackZ = 10.0f;

void GCollisionManager::Init()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	//gPvd = PxCreatePvd(*gFoundation);
	//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	//gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true);

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	//sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);

	//PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	//if (pvdClient)
	//{
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	//}
	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

	//PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	//gScene->addActor(*groundPlane);

	//for (PxU32 i = 0; i < 5; i++)
		//createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	//if (!interactive)
		//createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
}