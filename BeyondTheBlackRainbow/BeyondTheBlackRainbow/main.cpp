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
#include "Text\Text.h"


int main() {

	Renderer* renderer = Renderer::getInstance();
	if (renderer->init() == -1){
		return -1;
	}
	
	GLDebug::registerDebugCallbacks();
	
	InputHandler* input = new InputHandler();

	Text* text = new Text();
	text->prepareText("Escape!", 60, 500, 100);
	
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
	
	SceneNode* transformNodeRoom = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 3, -3, 1));
	//glm::rotate(transformNodeRoom->propagateMatrix(), 90.0f , glm::vec3(1, 0, 0));
	SceneNode* transformNodeDuck = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 3, -3, 1));
	SceneNode* transformNodeBed = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 3, -3, 1));
	SceneNode* transformNodeTable = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 3, -3, 1));
	SceneNode* transformNodeDoor = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 3, -3, 1));
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
	sceneGraph->attachChild(activeCamera);
	//end of part that should be in a scene loader
	
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
			sceneGraph->update(timeStep, input);
		}
		oldTime = time - deltaTime;

		glm::mat4 projectionMatrix = activeCamera->getProjectionMatrix();
		glm::mat4 viewMatrix = activeCamera->getViewMatrix();
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		for (MeshNode* node : drawArray){
			node->draw(viewMatrix, projectionMatrix, viewProjectionMatrix);
		}
		renderer->drawText(text);

		glfwSwapBuffers(renderer->getWindow());
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}