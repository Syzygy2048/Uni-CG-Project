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


int main() {

	Renderer* renderer = Renderer::getInstance();
	if (renderer->init() == -1){
		return -1;
	}
	
	GLDebug::registerDebugCallbacks();
	
	InputHandler* input = new InputHandler();
	
	CameraNode* activeCamera = new CameraNode(generateUuid());
	std::map<std::string, CameraNode*> cameraList;
	
	//this way we have a list of cameras and can switch between them as we want just by doing activeCamera = cameraList.find("whichever camera we want")->second;
	cameraList.insert(std::pair<std::string, CameraNode*>(std::string("player camera"), activeCamera));


	//start of part that should be in a scene loader
	MeshNode* tableMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	MeshNode* duckMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	tableMesh->prepareForRendering();
	duckMesh->prepareForRendering();

	std::vector<MeshNode*> drawArray;
	drawArray.push_back(tableMesh);
	drawArray.push_back(duckMesh);
	
	SceneNode* sceneGraph = new SceneNode(generateUuid(), NodeType::ROOT_NODE);
	sceneGraph->setParent(nullptr);
	
	SceneNode* transformNode = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		3, 3, -3, 1));
	transformNode->attachChild(tableMesh);
	sceneGraph->attachChild(duckMesh);
	sceneGraph->attachChild(transformNode);
	sceneGraph->attachChild(activeCamera);
	

	//end of part that should be in a scene loader
	
	//should be part of model loading
	//GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	//should be in a shader class
	//GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	double time = glfwGetTime();
	double oldTime = glfwGetTime();
	double timeStep = 1.0 / 60.0;
	//gameloop
	double timeOld = 0;
	while (!input->esc && glfwWindowShouldClose(renderer->getWindow()) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//start of part that should be part of individual rendering
		// Use our shader
		
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

		glfwSwapBuffers(renderer->getWindow());
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}