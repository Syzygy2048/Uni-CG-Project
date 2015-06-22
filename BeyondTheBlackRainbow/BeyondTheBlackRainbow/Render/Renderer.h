#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "../InputHandler.h"
#include "../Texture/Texture.h"
#include "../SceneGraph/PlayerNode.h"
#include "../SceneGraph/SceneGraphEnum.h"
#include "../SceneGraph/SceneNode.h"
#include "../SceneGraph/TransformNode.h"
#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/CameraNode.h"
#include "../SceneGraph/LightNode/LightNode.h"
#include "../Text/Text.h"
#include "../Framebuffer.h"
#include "../Shader/BloomShaderProgram.h"
#include "../Shader/RenderSurfaceShaderProgram.h"
#include "../Shader/HighPassShaderProgram.h"
#include "../Shader/BlurShaderProgram.h"
#include "../Shader/DepthOfFieldShaderProgram.h"


class Renderer
{
public:
	static Renderer* getInstance();
	~Renderer();

	int init(int viewPortResX, int viewPortResY);

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

	void generateFramebuffer(GLuint* id);
	void bindFramebuffer(GLuint id, int viewPortResX, int viewPortRexY, GLenum frameBufferTarget);
	
	void genRenderTexture(GLuint* id);
	void bindRenderTexture(GLuint id, int viewPortResX, int viewPortResY);

	void genDepthBuffer(GLuint* id);
	void bindDepthBuffer(GLuint id, int viewPortResX, int viewPortResY);

	void configureFramebufferForPostProcessing(int viewPortResX, int viewPortResY);

	void applyHighPassFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture);
	void applyBlurFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture);
	void applyBloomFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture);
	void applyDepthOfFieldFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture, GLuint depthTexture);

	void preparePostProcessing(int viewPortResX, int viewPortResY);
	void renderToScreen(int viewPortResX, int viewPortResY);
	
	void draw(MeshNode* node);
	void drawText(Text* text, bool enableBlend);
	void drawShadow(MeshNode* node, Framebuffer* framebuffer);

	GLFWwindow* getWindow();
	void setLights(std::vector<LightNode*> lights);

	void setFrameBuffers(std::map<std::string, Framebuffer*> framebuffers);
	void setDepthProjectionMatrix(glm::mat4 depthProjectionMatrix);
	void setDepthModelMatrix(glm::mat4 depthModelMatrix);

private:
	//singleton
	Renderer();
	Renderer(Renderer const&);
	void operator=(Renderer const&);

	
	GLuint renderFrameBuffer;
	GLuint renderDepthBuffer;
	GLuint dummyDepthTexture;
	GLuint renderSurfaceVAO;
	GLuint renderSurfaceVBO;
	GLuint renderTexture;
	GLuint renderTexture2;
	GLuint highPassTexture;
	
	HighPassShaderProgram* highPassShader;
	RenderSurfaceShaderProgram* renderSurfaceShader;
	BloomShaderProgram* bloomShader;
	BlurShaderProgram* blurShader;
	DepthOfFieldShaderProgram* dofShader;
	
	
	GLFWwindow* window;
	std::map<std::string, Framebuffer*> framebuffers;
	std::vector<LightNode*> lights;

	void useShader(MeshNode* node, std::vector<LightNode*> lights);
	void useShader(Text* text);
	void useShader(Framebuffer* framebuffer, MeshNode* node);
	
	//shadowmap for directional light
	std::vector<LightNode*> getLights(MeshNode* node);
	glm::mat4 depthProjectionMatrix;
	glm::mat4 depthModelMatrix;

	int viewPortResX;
	int viewPortResY;
};

