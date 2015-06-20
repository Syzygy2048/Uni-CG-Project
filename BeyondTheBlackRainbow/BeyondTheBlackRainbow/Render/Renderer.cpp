#include "Renderer.h"

#include <iostream>
#include "../Importers/ShaderImporter.h"

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

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

void Renderer::bindFramebuffer(GLuint id, int viewPortResX, int viewPortRexY, GLenum frameBufferTarget)
{
	glBindFramebuffer(frameBufferTarget, id);
	glViewport(0, 0, viewPortResX, viewPortRexY);

	
	//createRenderSurface(viewPortResX, viewPortRexY);

	GLenum status = glCheckFramebufferStatus(frameBufferTarget);

	if (status != GL_FRAMEBUFFER_COMPLETE){
		std::cerr << "error binding framebuffer: " << status << std::endl;
	}
		//clear previous image after binding to prevent stuff from drawing on itself
}

void Renderer::genRenderTexture(GLuint* id)
{
	glGenTextures(1, id);
}

void Renderer::bindRenderTexture(GLuint id, int viewPortResX, int viewPortResY)
{
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewPortResX, viewPortResY, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::genDepthBuffer(GLuint* id)
{
	glGenRenderbuffers(1, id);
}

void Renderer::bindDepthBuffer(GLuint id, int viewPortResX, int viewPortResY)
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
}

void Renderer::configureFramebufferForPostProcessing(int viewPortResX, int viewPortResY)
{
	//bindRenderTexture(renderTexture, viewPortResX, viewPortResY);
	bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_DRAW_FRAMEBUFFER);
	bindDepthBuffer(renderDepthBuffer, viewPortResX, viewPortResY);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//glActiveTexture(GL_TEXTURE0);
	//bindRenderTexture(renderTexture, viewPortResX, viewPortResY);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
}

void Renderer::createRenderSurface(int viewPortResX, int viewPortResY)
{

	generateFramebuffer(&renderFrameBuffer);
	bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_FRAMEBUFFER);

	genRenderTexture(&renderTexture2);
	bindRenderTexture(renderTexture2, viewPortResX, viewPortResY);

	genRenderTexture(&highPassTexture);
	bindRenderTexture(highPassTexture, viewPortResX, viewPortResY);

	genRenderTexture(&renderTexture);
	bindRenderTexture(renderTexture, viewPortResX, viewPortResY);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);

	GLfloat renderSurfaceVertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	



	genDepthBuffer(&renderDepthBuffer);
	bindDepthBuffer(renderDepthBuffer, viewPortResX, viewPortResY);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewPortResX, viewPortResY);
		
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);

	
	const int numberOfDrawbuffers = 1;
	GLenum drawBuffers[numberOfDrawbuffers] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(numberOfDrawbuffers, drawBuffers);

	generateVertexArray(&renderSurfaceVAO);
	generateBufferObject(&renderSurfaceVBO);

	bindVertexArray(renderSurfaceVAO);
	fillBuffer(renderSurfaceVBO, GL_ARRAY_BUFFER, sizeof(renderSurfaceVertices), &renderSurfaceVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, renderTexture);
	
	highPassShader = (HighPassShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::HIGH_PASS);
	highPassShader->loadUniformLocations();
	renderSurfaceShader = (RenderSurfaceShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::RENDER_SURFACE);
	renderSurfaceShader->loadUniformLocations();
	postProcessingShader = (BloomShaderProgram*) ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::BLOOM_SHADER);
	postProcessingShader->loadUniformLocations();
	
	bindVertexArray(0);
	bindBuffer(GL_ARRAY_BUFFER, 0);
	bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::applyHighPassFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture)
{
	bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_FRAMEBUFFER);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, targetTexture, 0);
	
	glUseProgram(highPassShader->getShaderId());
	highPassShader->fillUniformLocation(sourceTexture);

	bindVertexArray(renderSurfaceVAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	bindVertexArray(0);

}

void Renderer::renderToScreen(int viewPortResX, int viewPortResY)
{
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//applyHighPassFilter(viewPortResX, viewPortResY, renderTexture, highPassTexture);

	for (int i = 0; i < 2; i++)
	{
		bool horizontal;
		GLuint textureToRender;
		if (i == 0)
		{
			bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_FRAMEBUFFER);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture2, 0);	//render into texture2
			horizontal = true;
			textureToRender = renderTexture;		//render from texture1
		}
		else
		{
			bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_FRAMEBUFFER);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);		//render into texture
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			horizontal = false;
			textureToRender = renderTexture2;	//render from texture2
		}
		
		glUseProgram(postProcessingShader->getShaderId());
		postProcessingShader->fillUniformLocation(textureToRender, horizontal, viewPortResX, viewPortResY);
	
		bindVertexArray(renderSurfaceVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
	
		bindVertexArray(0);
	}
	glDisable(GL_BLEND);
	bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_READ_FRAMEBUFFER);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture2, 0);
	bindFramebuffer(0, viewPortResX, viewPortResY, GL_DRAW_FRAMEBUFFER);
	glUseProgram(renderSurfaceShader->getShaderId());
	
	renderSurfaceShader->fillUniformLocation(renderTexture);
	bindVertexArray(renderSurfaceVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
	
	bindVertexArray(0);
}

void Renderer::genrateShadowMapTexture(GLuint* id)
{
	glGenTextures(1, id);
}

void Renderer::glBindShadowMapTexture(GLuint id, int viewPortResX, int viewPortResY)
{
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, viewPortResX, viewPortResY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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