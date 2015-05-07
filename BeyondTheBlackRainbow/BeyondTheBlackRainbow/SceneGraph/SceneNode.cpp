#include "SceneNode.h"

#include <rpc.h>

#include <iostream>

SceneNode::SceneNode(UUID uuid, NodeType type) : uuid(uuid), nodeType(type)
{
	parent = nullptr;
}

SceneNode::~SceneNode()
{
	for (SceneNode* child : childList)
	{
		delete child;
	}
}

void SceneNode::update(double timeStep, InputHandler* input)
{
	for (SceneNode* child : childList)
	{
		child->update(timeStep, input);
	}
}

void SceneNode::attachChild(SceneNode* child)
{
	childList.push_back(child);
	child->setParent(this);
} 

//This leaves the empty transform nodes hanging which is kind of bad, should be refactored to also take out the empty transform nodes.
bool SceneNode::detachChild(SceneNode* deleteChild)
{
	for (std::vector<SceneNode*>::iterator iter = childList.begin(); iter != childList.end(); iter++)
	{
		if (*iter == deleteChild)
		{
			childList.erase(iter);
			delete *iter;
			return true;
		}
		return (*iter)->detachChild(deleteChild);
	}
	return false;
}

glm::highp_mat4 SceneNode::propagateMatrix()
{
	return glm::highp_mat4();
}

void SceneNode::setParent(SceneNode* parent)
{
	this->parent = parent;
}

NodeType SceneNode::getType()
{
	return nodeType;
}

UUID* SceneNode::getUuid()
{
	return &uuid;
}

bool SceneNode::receiveEvent(UUID target, EventTrigger eventTrigger, SceneNode* caller)
{
	bool* status = new bool;
	if (UuidEqual(&target, &uuid, (RPC_STATUS*)&status))
	{
		triggerEvent(eventTrigger, caller);
		delete status;
		return true;
	}
	for (SceneNode* child : childList)
	{
		if (child->receiveEvent(target, eventTrigger, caller))
		{
			return true;
		}
	}
	
	//delete status;
	return false;
}

bool SceneNode::receiveSpecificEvent(EventTrigger eventTrigger, EventIdentifier eventIdentifier, SceneNode* caller)
{
	{
		bool* status = new bool;
		for (Event* event : eventList)
		{
			if (event->trigger == eventTrigger && event->eventIdentifier == eventIdentifier){
				event->executeEvent(caller);
				return true;
			}
		}
		for (SceneNode* child : childList)
		{
			if (child->receiveSpecificEvent(eventTrigger, eventIdentifier, caller))
			{
				return true;
			}
		}

		//delete status;
		return false;
	}
}

void SceneNode::setEventManager(EventManager* eventManager)
{
	this->eventManager = eventManager;
}

void SceneNode::triggerEvent(EventTrigger eventTrigger, SceneNode* caller)
{
	for (Event* event : eventList)
	{
		if (event->trigger == eventTrigger){
			event->executeEvent(caller);
		}
	}
}

void SceneNode::registerEvent(Event* event){
	eventList.push_back(event);
	event->setEventTarget(this);
}


EventManager* SceneNode::getEventManager()
{
	return eventManager;
}