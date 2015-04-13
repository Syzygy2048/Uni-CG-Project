#pragma once

#include <assimp\scene.h>

#include "SceneNode.h"

class MeshNode :
	public SceneNode
{
public:
	MeshNode(UUID uuid, aiMesh* triangleMesh);
	~MeshNode();

	virtual glm::mat4 propagateMatrix();
private:
	aiMesh* triangleMesh;
};

