#pragma once

#include "SceneNode.h"

class LightNode : public SceneNode
{
public:
	LightNode(UUID uuid, glm::vec3 position, glm::vec3 intensity, glm::vec3 color, glm::vec3 direction);
	~LightNode();

	glm::mat4 propagateMatrix();

	glm::vec3 getPosition();
	glm::vec3 getIntensity();
	glm::vec3 getColor();
	glm::vec3 getDirection();

private:
	glm::vec3 position;
	glm::vec3 intensity;
	glm::vec3 color;
	glm::vec3 direction;
};

