#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

	int init();
	void initCamera();

	GLFWwindow* getWindow();

private:
	GLFWwindow* window;
};

