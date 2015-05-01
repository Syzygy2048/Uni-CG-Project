#include "PhysicsHandler.h"


PhysicsHandler::PhysicsHandler()
{
}


void PhysicsHandler::initPhysics()
{
/*	physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallback, defaultErrorCallback);
	physicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale());
	if (!physicsSDK)
	{
		std::cerr << "Error initializing PhysX3" << std::endl;
		return -1;
	}
	physx::PxCooking* cooking = PxCreateCooking(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!cooking)
	{
		std::cerr << "Error initializing PhysX3 cooking" << std::endl;
		return -1;
	}


	physx::PxSceneDesc sceneDesc(physicsSDK->getTolerancesScale());

	sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	physicsScene = physicsSDK->createScene(sceneDesc);
	physicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);	//for physx debug visualization


	physx::PxVisualDebuggerConnection* pvdConnection = NULL;
	*/
}

PhysicsHandler::~PhysicsHandler()
{
}
