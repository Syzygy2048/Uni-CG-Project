#pragma once

#include <windows.h>
#include <vector>
#include <glm\glm.hpp>
#include <rpc.h>

#include "SceneGraphEnum.h"
#include "../InputHandler.h"

#include "../Event/EventEnums.h"
class EventManager;
#include "../Event/EventManager.h"
class Event;
#include "../Event/Event.h"
class Behavior;
#include "../Behavior/Behavior.h"

class SceneNode
{
public:
	SceneNode(UUID id, NodeType nodeType);
	~SceneNode();

	void setParent(SceneNode* parent);
	SceneNode* getParent();
	virtual void update(double timeStep, InputHandler* input);
	//virtual void draw();
	virtual glm::highp_mat4 propagateMatrix();
	void attachChild(SceneNode* child);
	bool detachChild(SceneNode* child);

	NodeType getType();

	UUID* getUuid();

	bool receiveEvent(UUID target, EventTrigger eventTrigger, SceneNode* caller);
	bool receiveSpecificEvent(EventTrigger eventTrigger, EventIdentifier eventIdentifier, SceneNode* caller);
	
	void setEventManager(EventManager* eventManager);

	void registerEvent(Event* event);

	std::vector<SceneNode*> getChildren();
	
	void setBehavior(Behavior* behavior);
	
	EventManager* getEventManager();
	SceneNode* parent;
		


protected:
	EventManager* eventManager;
	std::vector<Event*> eventList;
	Behavior* behavior;

private:
	std::vector<SceneNode*> childList;	

	UUID uuid;
	const NodeType nodeType;

	virtual void triggerEvent(EventTrigger eventTrigger, SceneNode* caller);

};

