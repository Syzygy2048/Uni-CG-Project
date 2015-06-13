#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>

#include "Shader/ShaderProgram.h"
#include "Texture.h"

class ShaderProgram;


class Framebuffer
{
public:
	Framebuffer(const MeshLoadInfo::LoadInfo* meshLoadInfo);
	~Framebuffer();

	void prepareFrameBuffer();
	void useFrameBuffer();

	ShaderProgram* getShaderProgram();
	Texture* getTexture();
	GLuint getFramebufferID();

	void setDepthMVP(glm::mat4 depthMVP);
	glm::mat4 getDepthMVP();

private:
	GLuint frameBufferID;
	const MeshLoadInfo::LoadInfo* loadInfo;
	Texture* texture;
	ShaderProgram* shaderProgram;
	glm::mat4 depthMVP;

};

