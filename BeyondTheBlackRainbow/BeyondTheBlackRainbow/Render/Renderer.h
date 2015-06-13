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

class Renderer
{
public:
	static Renderer* getInstance();
	~Renderer();

	int init(int viewPortResX, int viewPortResY);

	void generateBufferObject(GLuint* bufferID);
	
	void fillBuffer(GLuint bufferID, GLenum bufferType, int bufferSize, GLvoid* bufferData, GLenum bufferUsage);
	void bindBuffer(GLenum bufferType, GLuint bufferID);

	void generateVertexArray(GLuint* vertexBufferID);
	void bindVertexArray(GLuint vertexArrayId);

	void enableVertexAttribArray(int id);
	void disableVertexAttribArray(int id);
	void setVertexAttribPointer(int id, int size, GLenum type, GLboolean normalized, int stride, const GLvoid* pointer);

	void linkShader(ShaderProgram* shader);

	void generateFramebuffer(GLuint* id);
	void bindFramebuffer(GLuint id, int viewPortResX, int viewPortRexY);

	void genrateShadowMapTexture(GLuint* id);
	void glBindShadowMapTexture(GLuint id, int viewPortResX, int viewPortRexY);

	void draw(MeshNode* node);
	void drawText(Text* text);

	GLFWwindow* getWindow();


private:
	//singleton
	Renderer();
	Renderer(Renderer const&);
	void operator=(Renderer const&);
	
	GLFWwindow* window;
	
	void useShader(MeshNode* node, std::vector<LightNode*> lights);
	void useShader(Text* text);
	std::vector<LightNode*> getLights(MeshNode* node);
};

