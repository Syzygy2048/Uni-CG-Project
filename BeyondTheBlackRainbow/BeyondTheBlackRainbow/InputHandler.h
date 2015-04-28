#pragma once
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <..\Camera.h>

class InputHandler
{
private:
	int w, a, s, d;
	double xpos, ypos;
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float speed = 3.0f;
	float mouseSpeed = 0.005f;

public:
	InputHandler();
	~InputHandler();
	void update(GLFWwindow* window, Camera* camera);
};

