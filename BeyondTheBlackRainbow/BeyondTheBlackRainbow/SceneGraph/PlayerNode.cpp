#include "PlayerNode.h"

#include "TransformNode.h"

PlayerNode::PlayerNode(UUID uuid) : SceneNode(uuid, NodeType::PLAYER_NODE)
{
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
	oldMousePosX = 640;
	oldMousePosY = 360;
}


PlayerNode::~PlayerNode()
{

}

void PlayerNode::createCollisionShape(PhysicsHandler* physicsHandler)
{
	playerController = physicsHandler->createPlayerActor(propagateMatrix());
	physicsHandler->addActorToScene(playerController->getActor());
	
	/*//init character controller https://developer.nvidia.com/sites/default/files/akamai/physx/Docs/CharacterControllers.html#character
	physx::PxControllerManager* manager = PxCreateControllerManager(*physicsScene);
	physx::PxCapsuleControllerDesc characterControllerDescription;
	//fill description here
	characterControllerDescription.position = physx::PxExtendedVec3(playerPosition->x, playerPosition->y, playerPosition->z);
	characterControllerDescription.radius = 0.5f;
	characterControllerDescription.height = 1.75f;
	characterControllerDescription.material = physicsSDK->createMaterial(0.001f, 0.001f, 0.01f);
	characterControllerDescription.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;

	physx::PxController* characterController = manager->createController(characterControllerDescription);
	characterController->getActor()->setName("player"); */
}

glm::mat4 PlayerNode::propagateMatrix()
{
	return parent->propagateMatrix();
}

void PlayerNode::update(double deltaTime, InputHandler* input)
{
	glm::vec3 disp(0.f, 0.f, 0.f);
	//if (input->r)
	//	controller->setPosition(physx::PxExtendedVec3(0, 50, 0));
	
	//std::cerr << "view matrix: " << glm::lookAt(position, right, up) << std::endl;
	//std::cerr << "model Matrix: " << propagateMatrix() << std::endl;

	position.x = playerController->getPosition().x;
	position.y = playerController->getPosition().y;
	position.z = playerController->getPosition().z;
	if (!touchingGround)
		yImpulse -= 0.098f * deltaTime;
	if (input->shift || !touchingGround)
		speed = 3.0;
	else
		speed = 2.0;
	if (input->w)
		disp += glm::normalize(glm::vec3(direction.x, 0.f, direction.z)) * (float)deltaTime * speed;
	if (input->s)
		disp += -glm::normalize(glm::vec3(direction.x, 0.f, direction.z)) * (float)deltaTime * speed;
	if (input->a)
		disp += -right * (float)deltaTime * speed;
	if (input->d)
		disp += right * (float)deltaTime * speed;
	if (input->space && touchingGround)
	{
		yImpulse = 0.04;
	}

	physx::PxControllerFilters filters;
	unsigned int collisionSide = 0;
	collisionSide = playerController->move(physx::PxVec3(disp.x, yImpulse, disp.z), 0.001f, deltaTime, filters);
	touchingGround = (collisionSide & physx::PxControllerFlag::eCOLLISION_DOWN) == physx::PxControllerFlag::eCOLLISION_DOWN;

	std::cerr << "direction before: " << direction.x << " " << direction.y << " " << direction.z << " ------ " << oldMousePosY - input->yPos << std::endl;

	//prevent camera from flipping
	if (direction.y > 0.9 && (oldMousePosY - input->yPos) > 0 || direction.y < -0.9 && oldMousePosY - input->yPos < 0)
	{
		updateDirection(deltaTime, oldMousePosX - input->xPos, 0);
	}
	else
	{
		updateDirection(deltaTime, oldMousePosX - input->xPos, oldMousePosY - input->yPos);
	}

	//updateDirection(horizontalAngle, verticalAngle);
	oldMousePosY = input->yPos;
	oldMousePosX = input->xPos;

	TransformNode* node;
	glm::mat4 parentTransform;
	if (parent->getType() == NodeType::TRANSFORM_NODE)
	{
		node = (TransformNode*)parent;
		node->setNewTransform(glm::lookAt(position, position + direction, up));
	}
	
}

void PlayerNode::updateDirection(double dT, float rotateX, float rotateY)
{
	horizontalAngle += turnSpeed * rotateX;
	verticalAngle += turnSpeed * rotateY;

	direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle));
	right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f));
	up = glm::cross(right, direction);

}

