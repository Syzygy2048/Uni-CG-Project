#pragma once

#include <glm\glm.hpp>

#include "../SceneGraph/SceneNode.h"

class TransformNode : public SceneNode
{
public:
	
	TransformNode(UUID uuid, glm::mat4 transform);
	~TransformNode();

	virtual glm::mat4 propagateMatrix();

	glm::mat4 getTransform();
	void setNewTransform(glm::mat4 newTransformMatrix);

private:
	glm::mat4 transformMatrix;

};

