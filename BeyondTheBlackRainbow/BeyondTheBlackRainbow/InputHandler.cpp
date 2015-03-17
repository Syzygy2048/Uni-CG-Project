#include "InputHandler.h"


InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::update(GLFWwindow* window)
{
	w = glfwGetKey(window, GLFW_KEY_W);
	a = glfwGetKey(window, GLFW_KEY_A);
	s = glfwGetKey(window, GLFW_KEY_S);
	d = glfwGetKey(window, GLFW_KEY_D);
	
}
