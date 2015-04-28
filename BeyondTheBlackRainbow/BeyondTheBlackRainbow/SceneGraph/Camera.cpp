#include "Camera.h"
#include <iostream>


Camera::Camera(UUID uuid) : SceneNode(uuid, NodeType::CAMERA_NODE)
{	
	position = glm::vec3(0, 0, 5);
	glm::vec3 up = glm::vec3(0, 1, 0);
	viewMatrix = glm::lookAt(position, position, up);
}

Camera::~Camera()
{
}

void Camera::updateDirection(float hAngle, float vAngle)
{
	direction = glm::vec3(cos(vAngle) * sin(hAngle),
		sin(vAngle),
		cos(vAngle) * cos(hAngle));
	right = glm::vec3(sin(hAngle - 3.14f / 2.2f),
		0,
		cos(hAngle - 3.14f / 2.2f));
	up = glm::cross(right, direction);
}

void Camera::move(float time, float speed, int dir)
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

glm::mat4 Camera::getViewMatrix() {
	viewMatrix = glm::lookAt(
		position,
		position + direction,
		up
		);
	return viewMatrix;
}

glm::mat4 Camera::propagateMatrix()
{
	return parent->propagateMatrix();
}