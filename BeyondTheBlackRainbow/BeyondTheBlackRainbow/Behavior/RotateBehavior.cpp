#include "RotateBehavior.h"
#include "../SceneGraph/TransformNode.h"

RotateBehavior::RotateBehavior() : Behavior()
{

}

RotateBehavior::~RotateBehavior()
{
}


void RotateBehavior::update(SceneNode* node, InputHandler* input, double deltaTime)
{
	TransformNode* transformNode = (TransformNode *)node->getParent();
	glm::highp_mat4 trans = transformNode->getTransform();

	//glm::highp_vec3 translation = glm::highp_vec3(trans[3][0], trans[3][1], trans[3][2]);
	glm::highp_vec3 translation = glm::highp_vec3(0, 0, -0.30);
	trans = glm::translate(trans, -translation);
	trans = glm::rotate(trans, -1., glm::highp_vec3(0, 1, 0));
	trans = glm::translate(trans, translation);
	transformNode->setNewTransform(trans);
}