#include "Framebuffer.h"

#include "Render\Renderer.h"
#include "Importers\ShaderImporter.h"

Framebuffer::Framebuffer(const MeshLoadInfo::LoadInfo* meshLoadInfo, int width, int height)
{
	loadInfo = meshLoadInfo;
	this->width = width;
	this->height = height;
}


Framebuffer::~Framebuffer()
{
}

void Framebuffer::prepareFrameBuffer(LightNode* myLight) 
{
	this->myLight = myLight;
	Renderer* renderer = Renderer::getInstance();
	renderer->generateFrameBuffer(&frameBufferID);
	texture = new Texture(width, height);
	if (myLight->getLightType() == DIRECTIONAL_LIGHT) {
		renderer->fillFrameBuffer(frameBufferID, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getTextureID(), 0);
	}
	else if (myLight->getLightType() == POINT_LIGHT) {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		renderer->fillFrameBuffer(frameBufferID, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getCubeMapID(), 0);
	}
	shaderProgram = ShaderImporter::getInstance()->loadShaderProgram(loadInfo->shaderInfo);
	shaderProgram->loadUniformLocations();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is not complete, repair me pls!" << std::endl;
	}
	renderer->bindFrameBuffer(GL_FRAMEBUFFER, 0);
}

ShaderProgram* Framebuffer::getShaderProgram()
{
	return shaderProgram;
}

Texture* Framebuffer::getTexture()
{
	return texture;
}

GLuint Framebuffer::getFramebufferID()
{
	return frameBufferID;
}

void Framebuffer::setDepthMVP(glm::mat4 depthMVP)
{
	this->depthMVP = depthMVP;
}

glm::mat4 Framebuffer::getDepthMVP()
{
	return depthMVP;
}

void Framebuffer::setDepthTransforms(std::vector<glm::mat4> depthTransforms)
{
	this->depthTransforms = depthTransforms;
}

std::vector<glm::mat4> Framebuffer::getDepthTransfomrs()
{
	return depthTransforms;
}

void Framebuffer::setFarPlane(float farPlane)
{
	this->farPlane = farPlane;
}

float Framebuffer::getFarPlane()
{
	return farPlane;
}
void Framebuffer::setNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
}

float Framebuffer::getNearPlane()
{
	return nearPlane;
}

LightNode* Framebuffer::getMyLight()
{
	return myLight;
}

