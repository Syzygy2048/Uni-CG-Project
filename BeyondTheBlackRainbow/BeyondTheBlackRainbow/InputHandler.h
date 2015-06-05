#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	void update(GLFWwindow* window);

	bool w, a, s, d, esc, space, shift, r, e, f2, f3;
	double xPos, yPos;
};

