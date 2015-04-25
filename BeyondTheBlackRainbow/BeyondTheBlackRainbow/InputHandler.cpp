#include "InputHandler.h"


InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

void InputHandler::update(GLFWwindow* window)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	/*w = glfwGetKey(window, GLFW_KEY_W);
	a = glfwGetKey(window, GLFW_KEY_A);
	s = glfwGetKey(window, GLFW_KEY_S);
	d = glfwGetKey(window, GLFW_KEY_D);*/

	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, 1280 / 2, 720 / 2); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	horizontalAngle += mouseSpeed * float(1280 / 2 - (1280-xpos));		//1280 - xpos to flip the x axis
	verticalAngle += mouseSpeed * float(720 / 2 - ypos);

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.2f),
		0,
		cos(horizontalAngle - 3.14f / 2.2f)
		);

	glm::vec3 up = glm::cross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	projectionMatrix = glm::perspective(initialFoV, 16.0f / 9.0f, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(
		position,
		position + direction,
		up
		);

	lastTime = currentTime;
}

glm::mat4 InputHandler::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 InputHandler::getViewMatrix()
{
	return viewMatrix;
}
/*
int InputHandler::getW()
{
return w;
}

int InputHandler::getA()
{
return a;
}

int InputHandler::getS()
{
return s;
}

int InputHandler::getD()
{
return d;
}
*/