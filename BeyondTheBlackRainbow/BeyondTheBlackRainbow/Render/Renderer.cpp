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

int Renderer::init(int viewPortResX, int viewPortResY)
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
	
	window = glfwCreateWindow(viewPortResX, viewPortResY, "Taste the Rainbow", NULL, NULL);

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
	//if (window == nullptr) //make sure it's already initialized
		//init(viewPortResX, viewPortResY);
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

void Renderer::draw(MeshNode* node)
{
		this->useShader(node, this->getLights(node));

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
	std::vector<LightNode*> lights;
	SceneNode* parentNode = node->parent;
	bool find = true;
	while (find) {
		if (parentNode != nullptr) {			
			std::vector<SceneNode*> children = parentNode->getChildren();
			for (int i = 0; i < children.size(); i++) {
				if (children.at(i)->getType() == LIGHT_NODE) {
					lights.push_back(dynamic_cast<LightNode*>(children.at(i)));
				}
			}
			parentNode = parentNode->parent;
		}
		else {
			find = false;
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
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	
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


void Renderer::generateFramebuffer(GLuint* id)
{
	glGenFramebuffers(1, id);
	
}

void Renderer::bindFramebuffer(GLuint id, int viewPortResX, int viewPortRexY)
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, viewPortResX, viewPortRexY);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		//clear previous image after binding to prevent stuff from drawing on itself
}

void Renderer::genrateShadowMapTexture(GLuint* id)
{
	glGenTextures(1, id);
}

void Renderer::glBindShadowMapTexture(GLuint id, int viewPortResX, int viewPortRexY)
{
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, viewPortResX, viewPortRexY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE){
		std::cerr << "error binding framebuffer: " << status << std::endl;
	}
}