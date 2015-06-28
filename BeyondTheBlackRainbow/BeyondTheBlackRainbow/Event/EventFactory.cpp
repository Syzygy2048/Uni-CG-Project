#include "EventFactory.h"


#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/SceneNode.h"

#include "../GameStateManager.h"


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
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				node->HAVE_KEY = true;
				GameStateManager::getInstance()->setPostProcessingEnabled(true);

				if (node->getFoundObject() == 2 && node->HAVE_KEY) {
					target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR, target);
				}
			}
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
		};
		return specificEvent;
		break;
	case EventIdentifier::DOOR_TRIGGER1:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "duck found" << std::endl;
				target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR_ROOM1, target);
			}
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR_ROOM1:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
		};
		return specificEvent;
		break;
	case EventIdentifier::LIGHT_FOUND:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE)
			{
				std::cout << "light found" << std::endl;
				MeshNode* node = (MeshNode*)target;
				node->LIGHT_FOUND = true;
			}
		};
		return specificEvent;
		break;
	case EventIdentifier::OBJECT_FOUND:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE)
			{
				std::cout << "object found" << std::endl;
				MeshNode* node = (MeshNode*)target;
				node->objectFound();
				node->setActiveTexture(nullptr);
				std::cout << node->getFoundObject() << std::endl;
				if (node->getFoundObject() == 2 && node->HAVE_KEY) {
					target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR, target);
				}
			}
		};
		return specificEvent;
		break;
	}
	return nullptr;
}



