#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "../SceneGraph/MeshNode.h"
#include "../InputHandler.h"
#include "../Texture.h"
#include "../SceneGraph/CameraNode.h"
#include "../Text/Text.h"

class Renderer
{
public:
	static Renderer* getInstance();
	~Renderer();

	int init();

	void generateBufferObject(GLuint* bufferID);
	
	void fillBuffer(GLuint bufferID, GLenum bufferType, int bufferSize, GLvoid* bufferData, GLenum bufferUsage);
	void bindBuffer(GLenum bufferType, GLuint bufferID);

	void generateVertexArray(GLuint* vertexBufferID);
	void bindVertexArray(GLuint vertexArrayId);

	void enableVertexAttribArray(int id);
	void disableVertexAttribArray(int id);
	void setVertexAttribPointer(int id, int size, GLenum type, GLboolean normalized, int stride, const GLvoid* pointer);

	void linkShader(ShaderProgram* shader);

	void draw(MeshNode* node);
	void drawText(Text* text);

	GLFWwindow* getWindow();


private:
	//singleton
	Renderer();
	Renderer(Renderer const&);
	void operator=(Renderer const&);
	
	GLFWwindow* window;
	
	void useShader(MeshNode* node);
};

