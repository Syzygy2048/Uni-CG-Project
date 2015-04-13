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

	void genBuffer(GLuint* bufferID);
	void bindBuffer(GLuint* bufferID);

	GLFWwindow* getWindow();

private:
	GLFWwindow* window;
};

