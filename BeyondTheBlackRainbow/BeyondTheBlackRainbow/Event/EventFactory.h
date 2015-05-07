#pragma once

#include "EventEnums.h"
#include "Event.h"
class EventFactory

{
public:
	static Event* createEvent(EventTrigger trigger, EventIdentifier idenfitifer);
private:
	EventFactory();
	~EventFactory();


};

