#pragma once

#include "SceneNode.h"
class MeshNode :
	public SceneNode
{
public:
	MeshNode(UUID* uuid, SceneNode* parent);
	~MeshNode();

	virtual glm::mat4 propagateMatrix();
};

