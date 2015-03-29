#include "MeshNode.h"


MeshNode::MeshNode(UUID* uuid, SceneNode* parent) : SceneNode(uuid, NodeType::MeshNode)
{
	this->parent = parent;
}


MeshNode::~MeshNode()
{
}

glm::mat4 MeshNode::propagateMatrix(){
	return parent->propagateMatrix();
}
