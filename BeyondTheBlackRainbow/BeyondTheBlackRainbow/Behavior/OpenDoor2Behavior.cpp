#include "OpenDoor2Behavior.h"
#include "../SceneGraph/TransformNode.h"



OpenDoor2Behavior::OpenDoor2Behavior() : Behavior()
{
	
}

OpenDoor2Behavior::~OpenDoor2Behavior()
{
}


void OpenDoor2Behavior::update(SceneNode* node, InputHandler* input, double deltaTime)
{
	if (triggered && rotateBy > 0){
		TransformNode* transformNode = (TransformNode *)node->getParent();
		glm::highp_mat4 trans = transformNode->getTransform();
		rotateBy -= rotationAmount;
		trans = glm::translate(trans, glm::highp_vec3(0, 0, 0.5));
		trans = glm::rotate(trans, -1., glm::highp_vec3(0, 1, 0));
		trans = glm::translate(trans, glm::highp_vec3(0, 0, -0.5));
		transformNode->setNewTransform(trans);
	}
}