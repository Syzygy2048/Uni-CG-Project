#pragma once

#include <rpc.h>

class SceneNode;
#include "../SceneGraph/SceneNode.h"
#include "EventEnums.h"

class EventManager
{
public:
	EventManager(SceneNode* sceneGraph);
	~EventManager();

	void registerEvent();

	//intended for first event call
	void eventTriggered(UUID target, EventTrigger event, SceneNode* caller);
	//intended for chained events
	void eventTriggered(EventTrigger event, EventIdentifier eventIdentifier, SceneNode* caller);

private:
	SceneNode* sceneGraph;
};

