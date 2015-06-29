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
				target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR, target);
				target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR_ROOM4, target);
			}
			//delete specificEvent;
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
			//delete specificEvent;
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
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::DOOR_TRIGGER6:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				if (node->HAVE_KEY) {
					std::cout << "door 6 found" << std::endl;
					target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR_ROOM6, target);
				}
				//delete specificEvent;
			}
		};
		return specificEvent;
		break;
	case EventIdentifier::_DOOR_TRIGGER6:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
					std::cout << "door _6 found" << std::endl;
					target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::_OPEN_DOOR_ROOM6, target);
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR_ROOM6:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "open door 6" << std::endl;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::_OPEN_DOOR_ROOM6:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "open door _6" << std::endl;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR_ROOM4:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "open door 4" << std::endl;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::DOOR_TRIGGER3:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "door 3 found" << std::endl;
				target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR_ROOM3, target);
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR_ROOM1:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "open door 1" << std::endl;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR_ROOM2:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "open door 2" << std::endl;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR_ROOM3:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "open door 3" << std::endl;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::OPEN_DOOR_ROOM5:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE) {
				MeshNode* node = (MeshNode*)target;
				std::cout << "open door 5" << std::endl;
				node->removeCollisionShape();
				if (behavior) behavior->setTriggered();
			}
			//delete specificEvent;
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
			//delete specificEvent;
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
				if (node->getFoundObject() == 2) {
					target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR_ROOM2, target);
				}
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	case EventIdentifier::VASE_FOUND:
		specificEvent->executeEvent = [specificEvent](SceneNode* node, Behavior* behavior)
		{
			SceneNode* target = specificEvent->getEventTarget();
			if (target->getType() == NodeType::MESH_NODE)
			{
				std::cout << "vase found" << std::endl;
				MeshNode* node = (MeshNode*)target;
				node->vaseFound();
				std::cout << node->getFoundVase() << std::endl;
				if (node->getFoundVase() == 3) {
					target->getEventManager()->eventTriggered(EventTrigger::EVENT, EventIdentifier::OPEN_DOOR_ROOM5, target);
				}
			}
			//delete specificEvent;
		};
		return specificEvent;
		break;
	}
	return nullptr;
}



