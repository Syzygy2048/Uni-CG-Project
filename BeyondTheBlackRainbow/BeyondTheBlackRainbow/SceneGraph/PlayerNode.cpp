#include "PlayerNode.h"

#include "TransformNode.h"
#include "../Util/DoFHelper.h"
#include "../GameStateManager.h"

PlayerNode::PlayerNode(int uuid) : SceneNode(uuid, NodeType::PLAYER_NODE)
{
	up = glm::vec3(0, 1, 0);
	right = glm::vec3(1, 0, 0);
	direction = glm::vec3(0, 0, 1);
	oldMousePosX = 640;
	oldMousePosY = 360;
}


PlayerNode::~PlayerNode()
{

}

void PlayerNode::createCollisionShape(PhysicsHandler* physicsHandler)
{
	playerController = physicsHandler->createPlayerActor(glm::mat4(propagateMatrix()));
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

glm::highp_mat4 PlayerNode::propagateMatrix()
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

	GameStateManager::getInstance()->updatePlayerPosition(position);
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
	glm::vec3 positionViewHack = glm::vec3(position.x, position.y + 1, position.z);	//bumps up the camera position by 1
	if (parent->getType() == NodeType::TRANSFORM_NODE)
	{
		node = (TransformNode*)parent;
		node->setNewTransform(glm::highp_mat4(glm::lookAt(positionViewHack, positionViewHack + direction, up)));
	}
	
	glm::vec3 cameraPosition = playerCamera->getPosition();
	physx::PxVec3 origin = physx::PxVec3(position.x, position.y + cameraPosition.y, position.z);                 // [in] Ray origin
	//glm::vec3 viewDirection = playerCamera->getDirect
	glm::vec3 normDir = glm::normalize(direction);
	physx::PxVec3 unitDir = physx::PxVec3(normDir.x, normDir.y, normDir.z);                // [in] Normalized ray direction
	physx::PxReal maxDistance = 300;            // [in] Raycast max distance
	physx::PxRaycastBuffer hit;                 // [out] Raycast results

	// Raycast against all static & dynamic objects (no filtering)
	// The main result from this call is the closest hit, stored in the 'hit.block' structure
	bool status = playerController->getScene()->raycast(origin, unitDir, maxDistance, hit);
	if (status){
		//std::cerr << origin.x << " " << origin.y << " ";
		DoFHelper::getInstance()->setDistanceToFocusedObject((origin - hit.block.position).magnitude());
		DoFHelper::getInstance()->update();
		if (input->e && hit.block.distance < 3)
		{
			if (!hit.block.actor->userData){
				std::cerr << "object name wasn't set" << std::endl;
				//std::cerr << "userdata " << hit.block.actor->userData << std::endl;
			}
			else
			{
				/*if (std::string("player").compare(std::string(hit.block.actor->getName())))
				{
				std::cerr << "raytrace hit the player, this is a bug, please aim more up or down to hit the actual object" << std::endl;
				} */
				eventManager->eventTriggered(*((int*)hit.block.actor->userData), EventTrigger::RAYTRACE_HIT, this);
			}
		}
	}

	if (position.y >= 3.6 && position.z < -14.5)
	{
		std::cerr << "YOU HAVE WON!" << std::endl;
	}
	//std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
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

glm::vec3 PlayerNode::getPosition()
{
	return position;
}

void PlayerNode::setCamera(CameraNode* camera){
	playerCamera = camera;
}
