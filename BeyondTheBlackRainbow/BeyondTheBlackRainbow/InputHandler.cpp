#include "InputHandler.h"


InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::update(GLFWwindow* window, Camera* camera)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, 1280 / 2, 720 / 2); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	horizontalAngle += mouseSpeed * float(1280 / 2 - (1280-xpos));		//1280 - xpos to flip the x axis
	verticalAngle += mouseSpeed * float(720 / 2 - ypos);

	camera->updateDirection(horizontalAngle, verticalAngle);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->move(deltaTime, speed, 1);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->move(deltaTime, speed, 2);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->move(deltaTime, speed, 3);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->move(deltaTime, speed, 4);
	}

	lastTime = currentTime;
}

