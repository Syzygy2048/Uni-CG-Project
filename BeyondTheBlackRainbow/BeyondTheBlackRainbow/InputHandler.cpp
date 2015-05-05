#include "InputHandler.h"


InputHandler::InputHandler()
{
	esc = false;
}


InputHandler::~InputHandler()
{
}

void InputHandler::update(GLFWwindow* window)
{
	glfwGetCursorPos(window, &xPos, &yPos);
	//glfwSetCursorPos(window, 1280 / 2, 720 / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	esc = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	r = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
}

