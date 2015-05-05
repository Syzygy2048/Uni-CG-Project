#include "TransformNode.h"


TransformNode::TransformNode(UUID uuid, glm::mat4 transform) : SceneNode(uuid, NodeType::TRANSFORM_NODE)
{
	transformMatrix = transform;
}


TransformNode::~TransformNode()
{
}

glm::mat4 TransformNode::propagateMatrix()
{
	return transformMatrix * parent->propagateMatrix();	
}

glm::mat4 TransformNode::getTransform()
{
	return transformMatrix;
}

void TransformNode::setNewTransform(glm::mat4 newTransformMatrix)
{
	transformMatrix = newTransformMatrix;
}