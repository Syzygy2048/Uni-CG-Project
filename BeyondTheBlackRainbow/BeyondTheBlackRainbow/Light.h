#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	Light(glm::vec3 position);
	~Light();

private:
	glm::vec3 position;
};

