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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	esc = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
	space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	r = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
	e = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
	f1 = glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS;
	f2 = glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS;
	f3 = glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS;
	f4 = glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS;
	f5 = glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS;
	f8 = glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS;
	f9 = glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS;
}

