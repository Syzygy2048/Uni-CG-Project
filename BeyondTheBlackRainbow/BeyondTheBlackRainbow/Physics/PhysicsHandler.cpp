#include "PhysicsHandler.h"

#include <iostream>
#include <GL\glew.h>


#include <vector>

#include <PxToolkit\PxToolkit.h>

PhysicsHandler::PhysicsHandler()
{
}

PhysicsHandler::~PhysicsHandler()
{
	physicsSDK->release();
	physicsFoundation->release();
}

void PhysicsHandler::initPhysics()
{
	physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, physxDefaultAllocatorCallback, physxDefaultErrorCallback);
	physicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale());
	if (!physicsSDK)
	{
		std::cerr << "Error initializing PhysX3" << std::endl;
		return;
	}
	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
	if (!cooking)
	{
		std::cerr << "Error initializing PhysX3 cooking" << std::endl;
		return;
	}

	physx::PxSceneDesc sceneDesc(physicsSDK->getTolerancesScale());

	sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	physicsScene = physicsSDK->createScene(sceneDesc);
	physicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);	//for physx debug visualization
	physicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 1.0f);
	physicsScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

	physx::PxVisualDebuggerConnection* pvdConnection = NULL; 
	
}

void PhysicsHandler::updatePhysics()
{
	physicsScene->simulate(physicsTimeStep);
	physicsScene->fetchResults(true);
}

//could and probably should be cached and reused like we do with most other things
physx::PxMaterial* PhysicsHandler::createPhysicsMaterial(float staticFriction, float dynamicFriction, float bounciness)
{
	return physicsSDK->createMaterial(staticFriction, dynamicFriction, bounciness);
}

void PhysicsHandler::renderCollisionShapes()
{
#ifdef _DEBUG
	const physx::PxRenderBuffer& rb = physicsScene->getRenderBuffer();
	for (physx::PxU32 i = 0; i < rb.getNbLines(); i++)
	{
		const physx::PxDebugLine& line = rb.getLines()[i];
		glLineWidth(1);
		glColor3f(1.0f, 0.1f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(line.pos0.x, line.pos0.y, line.pos0.z);
		glVertex3f(line.pos1.x, line.pos1.y, line.pos1.z);
		glEnd();
	}
#endif
}

physx::PxShape* PhysicsHandler::createSphereShape()
{
	return physicsSDK->createShape(physx::PxSphereGeometry(0.05f), *createPhysicsMaterial(90, 90, 0.9));
}

physx::PxRigidActor* PhysicsHandler::createRigidActor(glm::mat4 modelMatrix, aiMesh* mesh, const MeshLoadInfo::LoadInfo* loadInfo)
{
	switch (loadInfo->collisionType)
	{
		case MeshLoadInfo::CollisionType::DYNAMIC: return createDynamicActor(modelMatrix, createCollisionShape(loadInfo, mesh), createPhysicsMaterial(0.01, 0.01, 0.1f), 0.3);
		case MeshLoadInfo::CollisionType::STATIC: return createStaticActor(modelMatrix, createCollisionShape(loadInfo, mesh), createPhysicsMaterial(0.01, 0.01, 0.1f), 0.3);
	}
	std::cerr << "Invlaid collision type for " << mesh->mName.C_Str() << std::endl;
	return nullptr;
}

physx::PxRigidActor* PhysicsHandler::createDynamicActor(glm::mat4 modelMatrix, physx::PxShape* collisionShape, physx::PxMaterial* collisionMaterial, physx::PxReal density)
{
//	collisionShape->setFlag(physx::PxShapeFlag::eVISUALIZATION, true);
	physx::PxRigidDynamic* actor = physicsSDK->createRigidDynamic(physx::PxTransform(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]));
	actor->attachShape(*collisionShape);
	physx::PxRigidBodyExt::updateMassAndInertia(*actor, density);
	actor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
	
	//physx::PxRigidActor* actor = physx::PxCreateDynamic(*physicsSDK, physx::PxTransform(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]), collisionShape->getGeometry().any, *collisionMaterial, density);
	return actor;
}

physx::PxRigidActor* PhysicsHandler::createStaticActor(glm::mat4 modelMatrix, physx::PxShape* collisionShape, physx::PxMaterial* collisionMaterial, float collisionDensity)
{
	physx::PxRigidStatic* actor = physicsSDK->createRigidStatic(physx::PxTransform(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]));
	actor->attachShape(*collisionShape);
	actor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);

	//physx::PxRigidActor* actor = physx::PxCreateDynamic(*physicsSDK, physx::PxTransform(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]), collisionShape->getGeometry().any, *collisionMaterial, density);
	return actor;
}

physx::PxShape* PhysicsHandler::createCollisionShape(const MeshLoadInfo::LoadInfo* loadInfo, aiMesh* mesh)
{
	switch (loadInfo->collisionShapeType)
	{
	case MeshLoadInfo::CollisionShapeType::CONVEX: return createConvexHullShape(mesh);
	case MeshLoadInfo::CollisionShapeType::SPHERE: return createSphereShape();
	case MeshLoadInfo::CollisionShapeType::TRIANGLE: return createTriangleMeshShape(mesh);
	}
	std::cerr << "Invlaid collision shape for " << mesh->mName.C_Str() << std::endl;
	return nullptr;
}

physx::PxShape* PhysicsHandler::createTriangleMeshShape(aiMesh* mesh)
{
	physx::PxVec3* triangleVerts = (physx::PxVec3*)malloc(sizeof(physx::PxVec3) * mesh->mNumVertices);
	
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D vert = mesh->mVertices[i];
		triangleVerts[i] = (physx::PxVec3(vert.x, vert.y, vert.z));
	}
	
	physx::PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = mesh->mNumVertices;
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.points.data = triangleVerts;

	std::vector<unsigned int> faces;
	for (int k = 0; k < mesh->mNumFaces; k++)
	{
		faces.push_back(mesh->mFaces[k].mIndices[0]);
		faces.push_back(mesh->mFaces[k].mIndices[1]);
		faces.push_back(mesh->mFaces[k].mIndices[2]);
	}

	meshDesc.triangles.count = faces.size() / 3;
	meshDesc.triangles.stride = 3 * sizeof(unsigned int);
	meshDesc.triangles.data = faces.data();
	
	PxToolkit::PxDefaultMemoryOutputStream outputBuffer;
	if (!cooking->cookTriangleMesh(meshDesc, outputBuffer))
	{
		std::cerr << "could not create triangle collision shape for mesh : " << mesh->mName.C_Str() << std::endl;
 		return NULL;
	}
	PxToolkit::PxDefaultMemoryInputData inputStream(outputBuffer.getData(), outputBuffer.getSize());
	physx::PxTriangleMesh* triangleMesh = physicsSDK->createTriangleMesh(inputStream);

	return physicsSDK->createShape(physx::PxTriangleMeshGeometry(triangleMesh), *createPhysicsMaterial(90, 90, 0.9));
}

physx::PxShape* PhysicsHandler::createConvexHullShape(aiMesh* mesh)
{
	physx::PxVec3* convexVerts = (physx::PxVec3*)malloc(sizeof(physx::PxVec3) * mesh->mNumVertices);

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D vert = mesh->mVertices[i];
		convexVerts[i] = (physx::PxVec3(vert.x, vert.y, vert.z));
	}

	physx::PxConvexMeshDesc convexDesc;
	convexDesc.points.count = mesh->mNumVertices;
	convexDesc.points.stride = sizeof(physx::PxVec3);
	convexDesc.points.data = convexVerts;
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;
	convexDesc.vertexLimit = 256;

	PxToolkit::PxDefaultMemoryOutputStream outputBuffer;
	if (!cooking->cookConvexMesh(convexDesc, outputBuffer))
	{
		std::cerr << "could not create convex hull for mesh : " << mesh->mName.C_Str() << std::endl;
		return NULL;
	}
	PxToolkit::PxDefaultMemoryInputData inputStream(outputBuffer.getData(), outputBuffer.getSize());
	physx::PxConvexMesh* convexMesh = physicsSDK->createConvexMesh(inputStream);

	return physicsSDK->createShape(physx::PxConvexMeshGeometry(convexMesh), *createPhysicsMaterial(90, 90, 0.9));

}

void PhysicsHandler::addActorToScene(physx::PxRigidActor* actor)
{
	physicsScene->addActor(*actor);
}

void PhysicsHandler::createPhysicsFloor()
{
	physx::PxRigidStatic* plane = physx::PxCreatePlane(*physicsSDK, physx::PxPlane(physx::PxVec3(0, 1, 0), 0), *createPhysicsMaterial(90, 90, 0.1f));
	plane->setName("floor");
	addActorToScene(plane);
}

physx::PxController* PhysicsHandler::createPlayerActor(glm::mat4 modelMatrix)
{
	//init character controller https://developer.nvidia.com/sites/default/files/akamai/physx/Docs/CharacterControllers.html#character
	physx::PxControllerManager* manager = PxCreateControllerManager(*physicsScene);
	physx::PxCapsuleControllerDesc characterControllerDescription;
	//fill description here
	characterControllerDescription.position = physx::PxExtendedVec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
	characterControllerDescription.radius = 0.25f;
	characterControllerDescription.height = 1.f;
	characterControllerDescription.material = physicsSDK->createMaterial(0.001f, 0.001f, 0.01f);
	characterControllerDescription.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;

	physx::PxController* characterController = manager->createController(characterControllerDescription);
	characterController->getActor()->setName("player");
	characterController->setStepOffset(0.01);

	return characterController;
}

/*physx::PxRigidActor* PhysicsHandler::createDynamicActor(glm::mat4 modelMatrix)
{
	return physx::PxRigidActor* actor = nullptr;

	//physicsSDK->createRigidDynamic();
	//physx::PxCreateDynamic(physicsSDK, physx::PxTransform(physx::PxVec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2])), physicsSDK->createConvexMesh());

	physx::PxControllerManager* manager = PxCreateControllerManager(*physicsScene);
	physx::PxCapsuleControllerDesc characterControllerDescription;
	//fill description here
	characterControllerDescription.position = physx::PxExtendedVec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
	characterControllerDescription.radius = 0.5f;
	characterControllerDescription.height = 1.25f;
	characterControllerDescription.material = physicsSDK->createMaterial(0.01f, 0.01f, 0.1f);
	characterControllerDescription.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;

	characterController = manager->createController(characterControllerDescription); 
} */

/*physx::PxRigidActor* PhysicsHandler::createMeshStaticActor(glm::mat4 modelMatrix, aiMesh* mesh)
{
	physx::PxRigidStatic* meshActor = physicsSDK->createRigidStatic(physx::PxTransform(0, 0, 0));
	physx::PxTriangleMeshDesc meshDesc;

	std::vector<physx::PxVec3> verts;
	for (glm::vec3 vert : mesh->mVertices)
	{
		physx::PxVec3 v(vert.x, vert.y, vert.z);
		verts.push_back(v);
	}

	meshDesc.points.count = verts.size();
	meshDesc.points.stride = sizeof(physx::PxVec3);
	meshDesc.points.data = verts.data();

	meshDesc.triangles.count = faces.size() / 3;
	meshDesc.triangles.stride = 3 * sizeof(unsigned int);
	meshDesc.triangles.data = faces.data();
	//std::cerr << faces.size() << std::endl;

	if (!meshDesc.isValid()) std::cerr << "meshdescriptor not working " << verts.size() << std::endl;




	//PxToolkit::createTriangleMesh32(physicsSDK, cooking, &meshDesc);
	PxToolkit::PxDefaultMemoryOutputStream writeBuffer;
	cooking->cookTriangleMesh(meshDesc, writeBuffer);
	PxToolkit::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	physx::PxTriangleMesh* triangleMesh = physicsSDK->createTriangleMesh(readBuffer);
	physx::PxShape* triangleMeshShape = meshActor->createShape(physx::PxTriangleMeshGeometry(triangleMesh), *physicsSDK->createMaterial(0.5f, 0.5f, 0.1f));
	scene->addActor(*meshActor);	//create shape creates and adds the shape to the actor, so it does not have to be bound again; 
} 

physx::PxRigidActor* PhysicsHandler::createConvexHullDynamicActor(glm::mat4 modelMatrix, aiMesh mesh)
{
}
*/