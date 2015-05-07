#pragma once

#include <PhysX\PxPhysicsAPI.h>

#include <assimp\mesh.h>
#include <glm\glm.hpp>
#include <iostream>

#include "../Importers/MeshLoadInfo.h"


class PhysicsHandler
{
public:
	PhysicsHandler();
	~PhysicsHandler();

	void initPhysics();

	void updatePhysics();

	physx::PxMaterial* createPhysicsMaterial(float staticFriction, float dynamicFriction, float bounciness);
	
	physx::PxShape* createCollisionShape(const MeshLoadInfo::LoadInfo* loadInfo, aiMesh* mesh);
	physx::PxShape* createSphereShape();
	physx::PxShape* createConvexHullShape(aiMesh* mesh);
	physx::PxShape* createTriangleMeshShape(aiMesh* mesh);

	
	physx::PxRigidActor* createRigidActor(glm::mat4 modelMatrix, aiMesh* mesh, const MeshLoadInfo::LoadInfo* loadInfo);
	physx::PxRigidActor* createDynamicActor(glm::mat4 modelMatrix, physx::PxShape* collisionShape, physx::PxMaterial* collisionMaterial, float collisionDensity);
	physx::PxRigidActor* createStaticActor(glm::mat4 modelMatrix, physx::PxShape* collisionShape, physx::PxMaterial* collisionMaterial, float collisionDensity);

	physx::PxController* createPlayerActor(glm::mat4 modelMatrix);

	void addActorToScene(physx::PxRigidActor* actor);
	void createPhysicsFloor();
	//this uses the outdated fixed render pipeline which we're technically not allowed to use, but it's just for debug output and not part of actual rendering.
	void renderCollisionShapes();

private:

	physx::PxDefaultErrorCallback physxDefaultErrorCallback;
	physx::PxDefaultAllocator physxDefaultAllocatorCallback;

	struct ErrorCallback : physx::PxErrorCallback
	{
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line){
			std::cerr << message << ", " << file << ":" << line << std::endl;
		}
	};

	physx::PxPhysics* physicsSDK;
	physx::PxFoundation* physicsFoundation;

	physx::PxScene* physicsScene = nullptr;
	physx::PxReal physicsTimeStep = 1.f / 60.f;
	physx::PxRigidDynamic*	rigidDynamics = nullptr;
	physx::PxCooking* cooking = nullptr;
};

