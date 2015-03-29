#pragma once
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class InputHandler
{
private:
	int w, a, s, d;
	double xpos, ypos;
	glm::vec3 position = glm::vec3(0, 0, 5);
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float initialFoV = 90.0f;
	float speed = 3.0f;
	float mouseSpeed = 0.005f;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

public:
	InputHandler();
	~InputHandler();
	void update(GLFWwindow* window);
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	/*int getW();
	int getA();
	int getS();
	int getD();*/
};

