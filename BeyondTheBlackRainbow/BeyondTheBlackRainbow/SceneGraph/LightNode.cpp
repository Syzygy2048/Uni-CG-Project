#include "LightNode.h"


LightNode::LightNode(UUID uuid, glm::vec3 position, glm::vec3 intensity, glm::vec3 color, glm::vec3 direction) : SceneNode(uuid, NodeType::LIGHT_NODE)
{
	this->position = position;
	this->intensity = intensity;
	this->color = color;
	this->direction = direction;
}

LightNode::~LightNode()
{
}

glm::highp_mat4 LightNode::propagateMatrix()
{
	return parent->propagateMatrix();
}

glm::vec3 LightNode::getPosition()
{
	return position;
}

glm::vec3 LightNode::getIntensity()
{
	return intensity;
}

glm::vec3 LightNode::getColor()
{
	return color;
}

glm::vec3 LightNode::getDirection()
{
	return direction;
}
