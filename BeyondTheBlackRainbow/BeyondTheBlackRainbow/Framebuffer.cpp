#include "Framebuffer.h"

#include "Render\Renderer.h"
#include "Importers\ShaderImporter.h"

Framebuffer::Framebuffer(const MeshLoadInfo::LoadInfo* meshLoadInfo)
{
	loadInfo = meshLoadInfo;
}


Framebuffer::~Framebuffer()
{
}

void Framebuffer::prepareFrameBuffer() 
{
	Renderer* renderer = Renderer::getInstance();
	renderer->generateFrameBuffer(&frameBufferID);
	texture = new Texture();
	renderer->fillFrameBuffer(frameBufferID, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture->getTextureID(), 0);
	shaderProgram = ShaderImporter::getInstance()->loadShaderProgram(loadInfo->shaderInfo);
	shaderProgram->loadUniformLocations();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is not complete, repair me pls!" << std::endl;
	}
	renderer->bindFrameBuffer(GL_FRAMEBUFFER, frameBufferID);
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

