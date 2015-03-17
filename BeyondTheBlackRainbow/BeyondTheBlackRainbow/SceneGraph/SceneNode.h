#pragma once

#include <windows.h>
#include <vector>
#include <glm\glm.hpp>

#include "SceneGraphEnum.h"

class SceneNode
{
public:
	SceneNode(UUID id, NodeType nodeType, SceneNode* sceneGrap);
	~SceneNode();

	virtual void update(double timeStep);
	virtual void draw();
	virtual void propagateMatrix() = 0;
	/*void attachChild(SceneNode* child);
	bool detachChild(SceneNode* child);*/

private:
	//std::vector<SceneNode*> childList;
	SceneNode* parent;

	const UUID objectId;
	const NodeType nodeType;
};

