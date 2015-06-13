#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "../InputHandler.h"
#include "../Texture.h"
#include "../SceneGraph/PlayerNode.h"
#include "../SceneGraph/SceneGraphEnum.h"
#include "../SceneGraph/SceneNode.h"
#include "../SceneGraph/TransformNode.h"
#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/CameraNode.h"
#include "../SceneGraph/LightNode.h"
#include "../Text/Text.h"
#include "../Framebuffer.h"

class Renderer
{
public:
	static Renderer* getInstance();
	~Renderer();

	int init();

	void generateBufferObject(GLuint* bufferID);
	void generateFrameBuffer(GLuint* bufferID);
	
	void fillBuffer(GLuint bufferID, GLenum bufferType, int bufferSize, GLvoid* bufferData, GLenum bufferUsage);
	void fillFrameBuffer(GLuint bufferID, GLenum bufferType, GLenum attachment, GLuint texture, GLuint level);
	void bindBuffer(GLenum bufferType, GLuint bufferID);
	void bindFrameBuffer(GLenum bufferType, GLuint bufferID);
	void unbindFrameBuffer(GLenum bufferType);

	void generateVertexArray(GLuint* vertexBufferID);
	void bindVertexArray(GLuint vertexArrayId);

	void enableVertexAttribArray(int id);
	void disableVertexAttribArray(int id);
	void setVertexAttribPointer(int id, int size, GLenum type, GLboolean normalized, int stride, const GLvoid* pointer);

	void linkShader(ShaderProgram* shader);

	void draw(MeshNode* node);
	void drawText(Text* text);
	void drawShadow(MeshNode* node, Framebuffer* framebuffer);

	GLFWwindow* getWindow();
	void setFrameBuffers(std::vector<Framebuffer*> framebuffers);
	void setLights(std::vector<LightNode*> lights);

	void setDepthProjectionMatrix(glm::mat4 depthProjectionMatrix);
	void setDepthModelMatrix(glm::mat4 depthModelMatrix);

private:
	//singleton
	Renderer();
	Renderer(Renderer const&);
	void operator=(Renderer const&);
	
	GLFWwindow* window;
	std::vector<Framebuffer*> frameBuffers;
	std::vector<LightNode*> lights;

	void useShader(MeshNode* node, std::vector<LightNode*> lights);
	void useShader(Text* text);
	void useShader(Framebuffer* framebuffer);
	std::vector<LightNode*> getLights(MeshNode* node);

	glm::mat4 depthProjectionMatrix;
	glm::mat4 depthModelMatrix;
};

