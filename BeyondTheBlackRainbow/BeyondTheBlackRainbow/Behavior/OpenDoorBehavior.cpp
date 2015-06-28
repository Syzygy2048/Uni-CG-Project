#include "OpenDoorBehavior.h"
#include "../SceneGraph/TransformNode.h"



OpenDoorBehavior::OpenDoorBehavior() : Behavior()
{
	
}

OpenDoorBehavior::~OpenDoorBehavior()
{
}


void OpenDoorBehavior::update(SceneNode* node, InputHandler* input, double deltaTime)
{
	if (triggered && rotateBy > 0){
		TransformNode* transformNode = (TransformNode *)node->getParent();
		glm::highp_mat4 trans = transformNode->getTransform();
		rotateBy -= rotationAmount;
		trans = glm::translate(trans, glm::highp_vec3(0.5, 0, 0));
		trans = glm::rotate(trans, -1., glm::highp_vec3(0, 1, 0));
		trans = glm::translate(trans, glm::highp_vec3(-0.5, 0, 0));
		transformNode->setNewTransform(trans);
	}
}