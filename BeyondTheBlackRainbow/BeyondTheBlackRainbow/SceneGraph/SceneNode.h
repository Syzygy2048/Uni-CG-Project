#pragma once

#include <windows.h>
#include <vector>
#include <glm\glm.hpp>

#include "SceneGraphEnum.h"

class SceneNode
{
public:
	SceneNode(UUID* id, NodeType nodeType);
	~SceneNode();

	//virtual void update(double timeStep);
	//virtual void draw();
	virtual glm::mat4 propagateMatrix() = 0;
	/*void attachChild(SceneNode* child);
	bool detachChild(SceneNode* child);*/

protected:
	SceneNode* parent;
private:
	//std::vector<SceneNode*> childList;	

	const UUID* objectId;
	const NodeType nodeType;
};

