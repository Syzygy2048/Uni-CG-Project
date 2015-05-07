#include "EventManager.h"


EventManager::EventManager(SceneNode* sceneGraph)
{
	this->sceneGraph = sceneGraph;
}


void EventManager::eventTriggered(UUID target, EventTrigger event, SceneNode* caller)
{
	sceneGraph->receiveEvent(target, event, caller);
}

void EventManager::eventTriggered(EventTrigger event, EventIdentifier eventIdentifier, SceneNode* caller)
{
	sceneGraph->receiveSpecificEvent(event, eventIdentifier, caller);
}

EventManager::~EventManager()
{
}
