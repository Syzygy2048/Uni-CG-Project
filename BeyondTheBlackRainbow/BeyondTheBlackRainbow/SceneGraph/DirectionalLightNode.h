#pragma once

#include "../SceneGraph/LightNode.h"

class DirectionalLightNode : public LightNode
{
public:
	DirectionalLightNode(UUID uuid, glm::vec3 position, GLfloat intensity, glm::vec3 color, glm::vec3 direction, LightType type);
	~DirectionalLightNode();

	glm::vec3 getDirection();

private:

	glm::vec3 direction;
};

