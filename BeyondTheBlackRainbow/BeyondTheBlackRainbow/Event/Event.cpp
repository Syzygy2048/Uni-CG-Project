#include "Event.h"


Event::Event(EventTrigger trigger, EventIdentifier identifier) : trigger(trigger), eventIdentifier(identifier)
{
	std::cerr << "event created : trigger is : " << trigger << std::endl;
}


Event::~Event()
{
}


void Event::setEventTarget(SceneNode* node)
{
	eventTarget = node;
}

SceneNode* Event::getEventTarget()
{
	return eventTarget;
}