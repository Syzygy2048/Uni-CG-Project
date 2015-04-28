#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "InputHandler.h"
#include "shader.hpp"
#include "Render\Renderer.h"
#include "GLDebug.h"
#include "Util\UuidHelper.h"
#include "Importers\MeshImporter.h"
#include "SceneGraph\TransformNode.h"
#include "Camera.h"

int main() {

	Renderer* renderer = Renderer::getInstance();
	if (renderer->init() == -1){
		return -1;
	}
	
	GLDebug::registerDebugCallbacks();
	
	InputHandler* input = new InputHandler();



	//start of part that should be in a scene loader
	MeshNode* tableMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	MeshNode* duckMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	tableMesh->prepareForRendering();
	duckMesh->prepareForRendering();

	std::vector<MeshNode*> drawArray;
	drawArray.push_back(tableMesh);
	drawArray.push_back(duckMesh);
	
	SceneNode* sceneGraph = new SceneNode(generateUuid(), NodeType::ROOT_NODE);
	
	SceneNode* transformNode = new TransformNode(generateUuid(), glm::mat4(
		1, 0, 0, 3,
		0, 1, 0, 3,
		0, 0, 1, -3,
		0, 0, 0, 1));
	transformNode->attachChild(tableMesh);
	sceneGraph->attachChild(duckMesh);
	sceneGraph->attachChild(transformNode);
	sceneGraph->setParent(nullptr);
	//end of part that should be in a scene loader
	
	//should be part of model loading
	//GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	//should be in a shader class
	//GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	//this is just to test the scenegraph and give the second mesh a different mvp
	glm::mat4 testMatrix = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1.5, 1.5, 1.5, 1);	//constructor is kind of weird in that its flipped accross the scale axis (so projection and position is flipped)
	
	double time = glfwGetTime();
	double oldTime = glfwGetTime();
	double timeStep = 1.0 / 60.0;
	//gameloop
	double timeOld = 0;
	while (glfwGetKey(renderer->getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(renderer->getWindow()) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//start of part that should be part of individual rendering
		// Use our shader
		
		//input.update(window);
		renderer->input(input);
		/*glm::mat4 Projection = input->getProjectionMatrix();
		glm::mat4 View = input->getViewMatrix();
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection*View*Model;*/
		//scenegraph test
		//glm::mat4 m2vp = Projection*View*testMatrix;
		
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		//end of part that should be part of individual rendering

		time = glfwGetTime();
		double deltaTime = time - oldTime;
		while (deltaTime > timeStep)
		{
			deltaTime -= timeStep;
			sceneGraph->update(timeStep);
		}
		oldTime = time;
		for (MeshNode* node : drawArray){
			node->draw();
			//just to test scenegraph, remove
			//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &m2vp[0][0]);
		}
		glfwSwapBuffers(renderer->getWindow());
		glfwPollEvents();
	}
	
	glfwTerminate();

	return 0;
}