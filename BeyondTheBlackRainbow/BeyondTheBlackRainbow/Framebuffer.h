#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>

#include "Shader/ShaderProgram.h"
#include "Texture.h"
#include "SceneGraph\LightsEnum.h"

class ShaderProgram;
class MeshNode;


class Framebuffer
{
public:
	Framebuffer(const MeshLoadInfo::LoadInfo* meshLoadInfo, int width, int height);
	~Framebuffer();

	void prepareFrameBuffer(LightType type);
	void useFrameBuffer();

	ShaderProgram* getShaderProgram();
	Texture* getTexture();
	GLuint getFramebufferID();

	void setDepthMVP(glm::mat4 depthMVP);
	void setDepthTransforms(std::vector<glm::mat4> depthTransforms);
	glm::mat4 getDepthMVP();
	std::vector<glm::mat4> getDepthTransfomrs();

	void setFarPlane(float farPlane);
	float getFarPlane();
	void setNearPlane(float nearPlane);
	float getNearPlane();

private:
	GLuint frameBufferID;
	const MeshLoadInfo::LoadInfo* loadInfo;
	Texture* texture;
	ShaderProgram* shaderProgram;
	glm::mat4 depthMVP;
	std::vector<glm::mat4> depthTransforms;
	int width;
	int height;
	float farPlane;
	float nearPlane;
};

