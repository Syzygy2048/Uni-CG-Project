#include "Renderer.h"

#include <iostream>

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

int Renderer::init(){
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//#if _DEBUG
		// Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//#endif

	window = glfwCreateWindow(1280, 720, "Taste the Rainbow", NULL, NULL);

	if (window == NULL) {
		std::cerr << "Failed to open window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.4f, 0.0f, 0.4f, 0.0f);

}

void Renderer::initCamera(){

}

GLFWwindow* Renderer::getWindow(){
	if (window == nullptr)
		init();
	return window;
}