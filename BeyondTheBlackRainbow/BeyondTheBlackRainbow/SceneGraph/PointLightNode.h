#pragma once

#include "../SceneGraph/LightNode.h"

class PointLightNode : public LightNode
{
public:
	PointLightNode(UUID uuid, glm::vec3 position, GLfloat intensity, glm::vec3 color, LightType type);
	~PointLightNode();

	glm::vec3 getDirection();
};

