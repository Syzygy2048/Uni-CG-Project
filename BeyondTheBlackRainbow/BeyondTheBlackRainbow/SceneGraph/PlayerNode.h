#pragma once

#include "../SceneGraph/SceneNode.h"
#include "../Physics/PhysicsHandler.h"

class PlayerNode :
	public SceneNode
{
public:
	PlayerNode(UUID uuid);
	~PlayerNode();

	void createCollisionShape(PhysicsHandler* physicsHandler);

	virtual void update(double timeStep, InputHandler* input);

	virtual glm::mat4 propagateMatrix();

private:
	physx::PxController* playerController = nullptr;
	
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	double oldMousePosX;
	double oldMousePosY;

	float yImpulse;
	bool touchingGround;


	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float speed = 3.0f;
	float turnSpeed = 0.001f;

	void updateDirection(double deltaTime, float hAngle, float vAngle);
	void move(float time, float speed, int dir);
};

