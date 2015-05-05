#include "CameraNode.h"
#include <iostream>


CameraNode::CameraNode(UUID uuid) : SceneNode(uuid, NodeType::CAMERA_NODE)
{	
	position = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	viewMatrix = glm::lookAt(position, position, up);
	projectionMatrix = glm::perspective(75.0f, 16.0f / 9.0f, 0.1f, 100.0f);
}

CameraNode::~CameraNode()
{
}


void CameraNode::update(double deltaTime, InputHandler* input)
{
	/*horizontalAngle += turnSpeed * deltaTime * float(1280 / 2 - input->xPos);		//1280 - xPos to flip the x axis

	verticalAngle += turnSpeed * deltaTime * float(720 / 2 - input->yPos);
	if (verticalAngle > 1.5f) {
		verticalAngle = 1.5f;
	}
	else if (verticalAngle < -1.5f) {
		verticalAngle = -1.5f;
	}

	updateDirection(horizontalAngle, verticalAngle);

	if (input->w) {
		move(deltaTime, speed, 1);
	}
	else if (input->s) {
		move(deltaTime, speed, 2);
	}
	else if (input->d) {
		move(deltaTime, speed, 3);
	}
	else if (input->a) {
		move(deltaTime, speed, 4);
	}*/


}
void CameraNode::updateDirection(float hAngle, float vAngle)
{
	direction = glm::vec3(cos(vAngle) * sin(hAngle),
		sin(vAngle),
		cos(vAngle) * cos(hAngle));
	right = glm::vec3(sin(hAngle - 3.14f / 2.2f),
		0,
		cos(hAngle - 3.14f / 2.2f));
	up = glm::cross(right, direction);
}

void CameraNode::move(float time, float speed, int dir)
{
	if (dir == 1) {
	position += direction * time * speed;
	}
	else if (dir == 2) {
		position -= direction * time * speed;
	}
	else if (dir == 3) {
		position += right * time * speed;
	}
	else if (dir == 4) {
		position -= right * time * speed;
	}
}

glm::mat4 CameraNode::getViewMatrix()
{
	return propagateMatrix();
	/*viewMatrix = glm::lookAt(
		position,
		position + direction,
		up
		);
	return viewMatrix; */
}

glm::mat4 CameraNode::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 CameraNode::propagateMatrix()
{
	return parent->propagateMatrix();
}