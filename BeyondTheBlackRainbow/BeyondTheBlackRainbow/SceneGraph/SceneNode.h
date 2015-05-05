#pragma once

#include <windows.h>
#include <vector>
#include <glm\glm.hpp>
#include <rpc.h>

#include "SceneGraphEnum.h"
#include "../InputHandler.h"

class SceneNode
{
public:
	SceneNode(UUID id, NodeType nodeType);
	~SceneNode();

	void setParent(SceneNode* parent);
	virtual void update(double timeStep, InputHandler* input);
	//virtual void draw();
	virtual glm::mat4 propagateMatrix();
	void attachChild(SceneNode* child);
	bool detachChild(SceneNode* child);

	NodeType getType();

protected:
	SceneNode* parent;
private:
	std::vector<SceneNode*> childList;	

	const UUID objectId;
	const NodeType nodeType;
};

