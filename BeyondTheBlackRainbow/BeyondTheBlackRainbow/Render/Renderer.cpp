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
#ifdef _DEBUG
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
#endif
	

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	
	window = glfwCreateWindow(1024, 768, "Taste the Rainbow", NULL, NULL);

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

	glClearColor(0.8f, 0.9f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

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
	enableVertexAttribArray(id);
	glVertexAttribPointer(id, size, type, normalized, stride, pointer);
}

void Renderer::generateBufferObject(GLuint* bufferID)
{
	glGenBuffers(1, bufferID);
}

void Renderer::generateFrameBuffer(GLuint* bufferID)
{
	glGenFramebuffers(1, bufferID);
}

void Renderer::bindBuffer(GLenum bufferType, GLuint bufferID)
{
	glBindBuffer(bufferType, bufferID);
}

void Renderer::bindFrameBuffer(GLenum bufferType, GLuint bufferID)
{
	glBindFramebuffer(bufferType, bufferID);
	glViewport(0, 0, 1024, 1024);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::unbindFrameBuffer(GLenum bufferType)
{
	glBindFramebuffer(bufferType, 0);
	glViewport(0, 0, 1024, 768);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::fillBuffer(GLuint bufferID, GLenum bufferType, int bufferSize, GLvoid* bufferData, GLenum bufferUsage)
{
	bindBuffer(bufferType, bufferID);
	glBufferData(bufferType, bufferSize, bufferData, bufferUsage);
}

void Renderer::fillFrameBuffer(GLuint bufferID, GLenum bufferType, GLenum attachment, GLuint texture, GLuint level)
{
	bindFrameBuffer(bufferType, bufferID);
	glFramebufferTexture(bufferType, attachment, texture, level);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

}

void Renderer::generateVertexArray(GLuint* vertexArrayID)
{
	glGenVertexArrays(1, vertexArrayID);
}

void Renderer::bindVertexArray(GLuint vertexArrayId)
{
	glBindVertexArray(vertexArrayId);
}

void Renderer::draw(MeshNode* node)
{
	//std::vector<LightNode*> lights = this->getLights(node);

	this->useShader(node, lights);

	bindVertexArray(node->getVao());

	glDrawElements(GL_TRIANGLES, node->getDrawSize(), GL_UNSIGNED_INT, (void*)0);	
	
	bindVertexArray(0);
	glUseProgram(0);
}


void Renderer::linkShader(ShaderProgram* shader)
{
	glUseProgram(shader->getShaderId());
}

void Renderer::useShader(MeshNode* node, std::vector<LightNode*> lights)
{
	ShaderProgram* shaderProgram = node->getShaderProgram();
	glUseProgram(shaderProgram->getShaderId());
	
	shaderProgram->fillUniformLocation(node, lights);
}

std::vector<LightNode*> Renderer::getLights(MeshNode* node)
{	
	for (int i = 0; i < lights.size(); i++) {
		if (lights.at(i)->getLightType() == DIRECTIONAL_LIGHT) { //this do not work currently
			glm::vec3 lightInvDir = (lights.at(i)->getDirection());
			glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir + node->getPlayerPosition(), glm::vec3(0, 0, 0) + node->getPlayerPosition(), glm::vec3(0, 1, 0));			
			node->setDepthBiasMVP(depthProjectionMatrix * depthViewMatrix * depthModelMatrix);
			node->setShadowMap(frameBuffers.at(i)->getTexture());
		}
		else if (lights.at(i)->getLightType() == POINT_LIGHT) {
			//node->setFramebuffer(frameBuffers.at(i));
		}
	}		
	//std::cout << "Sum of lights: " << lights.size() << std::endl;
	return lights;
}

void Renderer::drawText(Text* text)
{
	this->useShader(text);

	bindVertexArray(text->getVAO());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, text->getVerticesSize());	
	glDisable(GL_BLEND);
	bindVertexArray(0);
	glUseProgram(0);

}

void Renderer::useShader(Text* text)
{
	ShaderProgram* shaderProgram = text->getShaderProgram();
	glUseProgram(shaderProgram->getShaderId());

	shaderProgram->fillUniformLocation(text);
}

void Renderer::drawShadow(MeshNode* node, Framebuffer* framebuffer)
{
	this->useShader(framebuffer, node);
	bindVertexArray(node->getVao());
	glDrawElements(GL_TRIANGLES, node->getDrawSize(), GL_UNSIGNED_INT, (void*)0);
	bindVertexArray(0);
	glUseProgram(0);
	//this->unbindFrameBuffer(GL_FRAMEBUFFER);
}

void Renderer::useShader(Framebuffer* framebuffer, MeshNode* node)
{
	ShaderProgram* shaderProgram = framebuffer->getShaderProgram();
	glUseProgram(shaderProgram->getShaderId());

	shaderProgram->fillUniformLocation(framebuffer);
	shaderProgram->fillUniformLocation(node, lights);
}


void Renderer::setFrameBuffers(std::vector<Framebuffer*> framebuffers)
{
	this->frameBuffers = framebuffers;
}

void Renderer::setLights(std::vector<LightNode*> lights)
{
	this->lights = lights;
}

void Renderer::setDepthProjectionMatrix(glm::mat4 depthProjectionMatrix)
{
	this->depthProjectionMatrix = depthProjectionMatrix;
}

void Renderer::setDepthModelMatrix(glm::mat4 depthModelMatrix)
{
	this->depthModelMatrix = depthModelMatrix;
}