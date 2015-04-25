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

#include "Importers\MeshImporter.h"

#define TIMESTEP 1/60

int main() {

	Renderer* renderer = Renderer::getInstance();
	if (renderer->init() == -1){
		return -1;
	}
	
	GLDebug::registerDebugCallbacks();

	InputHandler* input = new InputHandler();

	MeshNode* tableMesh = MeshImporter::getInstance()->getMesh(MeshLoadInfo::DUCK);
	tableMesh->prepareForRendering();
		
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	//gameloop
	double timeOld = 0;
	while (glfwGetKey(renderer->getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(renderer->getWindow()) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double timeNew = glfwGetTime();
		double timeDelta = timeNew - timeOld;
		
		// Use our shader
		glUseProgram(programID);
		
		//input.update(window);
		input->update(renderer->getWindow());
		glm::mat4 Projection = input->getProjectionMatrix();
		glm::mat4 View = input->getViewMatrix();
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection*View*Model;		
		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		
		
		
		tableMesh->draw();
		
		glfwSwapBuffers(renderer->getWindow());
		glfwPollEvents();
	}

	// Cleanup VBO and shader


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}