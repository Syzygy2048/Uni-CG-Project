#include "Renderer.h"

#include <iostream>

Renderer::Renderer()
{
}


Renderer* Renderer::getInstance(){
	static Renderer instance;
	return &instance;
}

Renderer::~Renderer()
{
}

int Renderer::init()
{
	if (!glfwInit()) 
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	
	window = glfwCreateWindow(1280, 720, "Taste the Rainbow", NULL, NULL);

	if (window == NULL)
	{
		std::cerr << "Failed to open window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.4f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

void Renderer::initCamera()
{

}

GLFWwindow* Renderer::getWindow()
{
	if (window == nullptr)
		init();
	return window;
}




void Renderer::enableVertexAttribArray(int id)
{
	glEnableVertexAttribArray(id);
}
void Renderer::disableVertexAttribArray(int id)
{
	glDisableVertexAttribArray(id);
}

void Renderer::setVertexAttribPointer(int id, int size, GLenum type, GLboolean normalized, int stride, const GLvoid* pointer)
{
	//bindBuffer(bufferType, bufferID);
	enableVertexAttribArray(id);
	glVertexAttribPointer(id, size, type, normalized, stride, pointer);
}

//possibly add buffer type when needed, or create specific functions for  buffer types. this might be wrong though as we technically went the renderer calls to be independant of OGL specific things like GLuint.
void Renderer::generateBufferObject(GLuint* bufferID)
{
	glGenBuffers(1, bufferID);
}


void Renderer::bindBuffer(GLenum bufferType, GLuint bufferID)
{
	glBindBuffer(bufferType, bufferID);
}

void Renderer::fillBuffer(GLuint bufferID, GLenum bufferType, int bufferSize, GLvoid* bufferData, GLenum bufferUsage)
{
	bindBuffer(bufferType, bufferID);
	glBufferData(bufferType, bufferSize, bufferData, bufferUsage);
}

void Renderer::generateVertexArray(GLuint* vertexArrayID)
{
	glGenVertexArrays(1, vertexArrayID);
}

void Renderer::bindVertexArray(GLuint vertexArrayId)
{
	glBindVertexArray(vertexArrayId);
}

void Renderer::draw(MeshNode* node, InputHandler* input)
{

	GLuint shaderID = node->getShaderID();
	this->useShader(shaderID);
	glm::mat4 MVP = this->getMVP(input);
	GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	Texture* texture = node->getTexture("duck.png");
	texture->bind(0);
	GLuint tex_location = glGetUniformLocation(shaderID, "first_texture");
	glUniform1i(tex_location, 0);

	bindVertexArray(node->getVao());
	glDrawElements(GL_TRIANGLES, node->getDrawSize(), GL_UNSIGNED_INT, (void*)0);
	bindVertexArray(0);

	texture->~Texture();
}

void Renderer::useShader(GLuint shaderID)
{
	glUseProgram(shaderID);
}

glm::mat4 Renderer::getMVP(InputHandler* input)
{
	glm::mat4 Projection = input->getProjectionMatrix();
	glm::mat4 View = input->getViewMatrix();
	glm::mat4 Model = glm::mat4(1.0f);
	return Projection*View*Model;
}