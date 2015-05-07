#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <map>

#include "InputHandler.h"
#include "shader.hpp"
#include "Render\Renderer.h"
#include "GLDebug.h"
#include "Util\UuidHelper.h"
#include "Importers\MeshImporter.h"
#include "SceneGraph\TransformNode.h"
#include "SceneGraph\CameraNode.h"
#include "SceneGraph\PlayerNode.h"
#include "Physics\PhysicsHandler.h"
#include "Text\Text.h"
#include "Event\EventFactory.h"

int main() {

	Renderer* renderer = Renderer::getInstance();
	if (renderer->init() == -1){
		return -1;
	}
	
	GLDebug::registerDebugCallbacks();
	
	InputHandler* input = new InputHandler();

	PhysicsHandler* physics = new PhysicsHandler();
	physics->initPhysics();

	Text* text = new Text();
	text->prepareText("Escape!", 60, 0, 0);
	

	std::map<std::string, CameraNode*> cameraList;

	//start of part that should be in a scene loader
	CameraNode* activeCamera = new CameraNode(generateUuid());
	
	
	//this way we have a list of cameras and can switch between them as we want just by doing activeCamera = cameraList.find("whichever camera we want")->second;
	cameraList.insert(std::pair<std::string, CameraNode*>(std::string("player camera"), activeCamera));
	
	MeshNode* tableMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::TABLE);
	MeshNode* duckMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	MeshNode* bedMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::BED);
	MeshNode* roomMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::ROOM);
	MeshNode* doorMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DOOR);
	tableMesh->prepareForRendering();
	duckMesh->prepareForRendering();
	bedMesh->prepareForRendering();
	roomMesh->prepareForRendering();
	doorMesh->prepareForRendering();

	std::vector<MeshNode*> drawArray;
	drawArray.push_back(tableMesh);
	drawArray.push_back(duckMesh);
	drawArray.push_back(bedMesh);
	drawArray.push_back(roomMesh);
	drawArray.push_back(doorMesh);

	SceneNode* sceneGraph = new SceneNode(generateUuid(), NodeType::ROOT_NODE);
	sceneGraph->setParent(nullptr);

	EventManager* eventManager = new EventManager(sceneGraph);
	sceneGraph->setEventManager(eventManager);
	tableMesh->setEventManager(eventManager);
	duckMesh->setEventManager(eventManager);
	bedMesh->setEventManager(eventManager);
	roomMesh->setEventManager(eventManager);
	doorMesh->setEventManager(eventManager);
	
	
	SceneNode* transformNodeRoom = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1));
	//glm::rotate(transformNodeRoom->propagateMatrix(), 90.0f , glm::vec3(1, 0, 0));
	SceneNode* transformNodeDuck = new TransformNode(generateUuid(), glm::mat4(
		0.1, 0, 0, 0,
		0, 0.1, 0, 0,
		0, 0, 0.1, 0,
		1, -0.1, -3.5, 1));
	SceneNode* transformNodeBed = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3.4, 0, -1.01, 1));
	SceneNode* transformNodeTable = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1));
	SceneNode* transformNodeDoor = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		2, 0, -5.1, 1));
	transformNodeRoom->attachChild(roomMesh);
	transformNodeRoom->attachChild(transformNodeDuck);
	transformNodeRoom->attachChild(transformNodeBed);
	transformNodeRoom->attachChild(transformNodeTable);
	transformNodeDuck->attachChild(duckMesh);
	transformNodeBed->attachChild(bedMesh);
	transformNodeTable->attachChild(tableMesh);
	transformNodeDoor->attachChild(doorMesh);
	
	sceneGraph->attachChild(transformNodeRoom);
	sceneGraph->attachChild(transformNodeDoor);
	
	SceneNode* playerTransform = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		2, 0, -3, 1));

	PlayerNode* player = new PlayerNode(generateUuid());
	playerTransform->attachChild(activeCamera);
	playerTransform->attachChild(player);
	sceneGraph->attachChild(playerTransform);
	player->setEventManager(eventManager);
	
	roomMesh->createCollisionShape(physics);
	player->createCollisionShape(physics);
	tableMesh->createCollisionShape(physics);
	duckMesh->createCollisionShape(physics);
	bedMesh->createCollisionShape(physics);
	doorMesh->createCollisionShape(physics);

	bedMesh->registerEvent(EventFactory::createEvent(EventTrigger::RAYTRACE_HIT, EventIdentifier::DOOR_TRIGGER));
	doorMesh->registerEvent(EventFactory::createEvent(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR));


	/*SceneNode* debugTransform = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		2, 3, -2.3, 1));
	MeshNode* debugMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	debugTransform->attachChild(debugMesh);
	sceneGraph->attachChild(debugTransform);
	drawArray.push_back(debugMesh);
	debugMesh->prepareForRendering();
	debugMesh->createCollisionShape(physics); */
	
	
	
	
	//should probably done recursively in sceneNode		
	
	//end of part that should be in a scene loader

	physics->createPhysicsFloor();
	
	double time = glfwGetTime();
	double oldTime = glfwGetTime();
	double timeStep = 1.0 / 60.0;
	
	//gameloop
	double timeOld = 0;
	while (!input->esc && glfwWindowShouldClose(renderer->getWindow()) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		input->update(renderer->getWindow());

		time = glfwGetTime();
		double deltaTime = time - oldTime;
		while (deltaTime > timeStep)
		{
			deltaTime -= timeStep;
			physics->updatePhysics();
			sceneGraph->update(timeStep, input);
		}
		oldTime = time - deltaTime;

		glm::mat4 projectionMatrix = activeCamera->getProjectionMatrix();
		glm::mat4 viewMatrix = activeCamera->getViewMatrix();
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		for (MeshNode* node : drawArray){
			node->draw(viewMatrix, projectionMatrix, viewProjectionMatrix);
		}

		glLoadMatrixf(&viewProjectionMatrix[0][0]);

		physics->renderCollisionShapes();
		
		renderer->drawText(text);
		
		glfwSwapBuffers(renderer->getWindow());
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}