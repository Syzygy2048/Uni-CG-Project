#pragma once


#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void updateDirection(float hAngle, float vAngle);
	void move(float time, float speed, int dir);
	glm::mat4 getViewMatrix();

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};

