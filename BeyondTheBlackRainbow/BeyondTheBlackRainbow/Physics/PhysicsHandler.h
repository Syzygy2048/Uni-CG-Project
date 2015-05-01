#pragma once

#include <PhysX\PxPhysicsAPI.h>

class PhysicsHandler
{
public:
	PhysicsHandler();
	~PhysicsHandler();

	void initPhysics();

private:

/*	physx::PxDefaultErrorCallback physxDefaultErrorCallback;
	physx::PxDefaultAllocator physxDefaultAllocatorCallback;

	struct ErrorCallback : physx::PxErrorCallback
	{
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line){
			//std::cout << message << ", " << file << ":" << line << std::endl;
		}
	};

	physx::PxPhysics* physicsSDK;
	physx::PxFoundation* physicsFoundation;

	physx::PxScene* physicsScene = NULL;
	physx::PxReal physicsTimeStep = 1.f / 60.f;
	physx::PxRigidDynamic*	rigidDynamics = NULL; */
};

