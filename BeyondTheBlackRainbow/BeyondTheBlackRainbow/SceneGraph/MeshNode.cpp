#include "MeshNode.h"


MeshNode::MeshNode(UUID uuid, aiMesh* triangleMesh) : SceneNode(uuid, NodeType::MESH_NODE)
{
	
}


MeshNode::~MeshNode()
{
}

glm::mat4 MeshNode::propagateMatrix(){
	return parent->propagateMatrix();
}
