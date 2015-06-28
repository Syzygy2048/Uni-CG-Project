#pragma once

#include "../InputHandler.h"
class SceneNode;
#include "../SceneGraph/SceneNode.h"

class Behavior
{
public:
	Behavior();
	~Behavior();

	virtual void update(SceneNode* node, InputHandler* input, double deltaTime) = 0;
	
	void setTriggered();

protected:
	bool triggered = false;

};

