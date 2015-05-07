#include "EventFactory.h"


#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/SceneNode.h"


EventFactory::EventFactory()
{
}


EventFactory::~EventFactory()
{
}


Event* EventFactory::createEvent(EventTrigger trigger, EventIdentifier identifier)
{
	Event* specificEvent = new Event(trigger, identifier);
	switch (identifier)
	{
	case EventIdentifier::DOOR_TRIGGER:
		specificEvent->executeEvent = [specificEvent](SceneNode* node)
		{
			SceneNode* target = specificEvent->getEventTarget();
			target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR, target);
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR:
		specificEvent->executeEvent = [specificEvent](SceneNode* node)
		{
			SceneNode* target = specificEvent->getEventTarget();

			if (target->getType() == NodeType::MESH_NODE)
			{
				std::cerr << "open door" << std::endl;
				MeshNode* node = (MeshNode*)target;
				node->removeCollisionShape();
				node->SUBMISSION1_ANIMATION_HACK = true;
			}
		};
		return specificEvent;
		break;
	}
	return nullptr;
}