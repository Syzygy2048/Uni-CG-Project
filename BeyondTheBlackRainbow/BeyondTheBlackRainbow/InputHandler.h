#pragma once
#include <GLFW\glfw3.h>

class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	void update(GLFWwindow* window);
private:
	int W, A, S, D;
};

