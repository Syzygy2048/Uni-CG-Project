#pragma once

#include <glm\glm.hpp>

#include "../SceneGraph/SceneNode.h"

class TransformNode : public SceneNode
{
public:
	
	TransformNode(UUID uuid, glm::mat4 transform);
	~TransformNode();

	virtual glm::mat4 propagateMatrix();

private:
	glm::mat4 transformMatrix;

};

