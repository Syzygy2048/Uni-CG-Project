#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	void update(GLFWwindow* window);

	bool w, a, s, d, esc, space, shift, r, e, f1, f2, f3, f4, f5, f8, f9;
	double xPos, yPos;
};

