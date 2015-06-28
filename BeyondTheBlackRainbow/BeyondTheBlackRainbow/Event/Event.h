#pragma once

#include "EventEnums.h"
#include <iostream>
#include <functional>

class SceneNode;
class Behavior;

class Event
{
public:
	Event(EventTrigger trigger, EventIdentifier eventIdentifier);
	~Event();

	const EventTrigger trigger;
	const EventIdentifier eventIdentifier;


	//function pointer. works like java observer/listener pattern
	std::function<void(SceneNode* caller, Behavior* behavior)> executeEvent;

	void setEventTarget(SceneNode* node);

	SceneNode* getEventTarget();
private:
	SceneNode* eventTarget;
};

