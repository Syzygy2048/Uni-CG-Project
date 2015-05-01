#pragma once

#include <PhysX\PxPhysicsAPI.h>

class PhysicsHandler
{
public:
	PhysicsHandler();
	~PhysicsHandler();

	void initPhysics();

private:
	struct ErrorCallback : physx::PxErrorCallback
	{
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line){
			//std::cout << message << ", " << file << ":" << line << std::endl;
		}
	};

	//static physx::PxPhysics* physicsSDK = NULL;
//	static physx::PxFoundation* physicsFoundation = NULL;

	//static physx::PxDefaultAllocator defaultAllocatorCallback;

	physx::PxScene* physicsScene = NULL;
	physx::PxReal physicsTimeStep = 1.f / 60.f;
	physx::PxRigidDynamic*	rigidDynamics = NULL;
};

