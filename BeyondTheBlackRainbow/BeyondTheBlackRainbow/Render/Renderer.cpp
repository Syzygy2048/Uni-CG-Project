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

	glViewport(0, 0, viewPortResX, viewPortResY);

	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.8f, 0.9f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
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
}

void Renderer::genRenderTexture(GLuint* id)
{
	glGenTextures(1, id);
}

void Renderer::bindRenderTexture(GLuint id, int viewPortResX, int viewPortResY)
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Renderer::genDepthBuffer(GLuint* id)
{
	glGenRenderbuffers(1, id);
}

void Renderer::bindDepthBuffer(GLuint id, int viewPortResX, int viewPortResY)
{
	glBindRenderbuffer(GL_RENDERBUFFER, id);	
}

void Renderer::preparePostProcessing(int viewPortResX, int viewPortResY)
{
	GLfloat quadVertices[] = {   // Vertex attributes for a quad that in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
		// Positions   // TexCoords
		-1.f, 1.0f, 0.0f, 1.0f,
		-1.f, -1.f, 0.0f, 0.0f,
		1.f, -1.f, 1.0f, 0.0f,

		-1.f, 1.0f, 0.0f, 1.0f,
		1.f, -1.f, 1.0f, 0.0f,
		1.f, 1.0f, 1.0f, 1.0f
	};
	
	glGenVertexArrays(1, &renderSurfaceVAO);
	glGenBuffers(1, &renderSurfaceVBO);
	glBindVertexArray(renderSurfaceVAO);
	glBindBuffer(GL_ARRAY_BUFFER, renderSurfaceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	glGenFramebuffers(1, &renderFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderFrameBuffer);

	glGenTextures(1, &renderTexture2);
	glBindTexture(GL_TEXTURE_2D, renderTexture2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewPortResX, viewPortResY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewPortResX, viewPortResY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

	glGenRenderbuffers(1, &renderDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewPortResX, viewPortResY);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderDepthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	

	renderSurfaceShader = (RenderSurfaceShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::RENDER_SURFACE);
	renderSurfaceShader->loadUniformLocations();
	

	highPassShader = (HighPassShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::HIGH_PASS);
	highPassShader->loadUniformLocations();
	
	postProcessingShader = (BloomShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::BLOOM_SHADER);
	postProcessingShader->loadUniformLocations();
	
}

void Renderer::configureFramebufferForPostProcessing(int viewPortResX, int viewPortResY)
{
	bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_DRAW_FRAMEBUFFER);
	glClearColor(0.7f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
	//bindDepthBuffer(renderDepthBuffer, viewPortResX, viewPortResY);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderToScreen(int viewPortResX, int viewPortResY)
{
	applyBloomFilter(viewPortResX, viewPortResY, renderTexture, renderTexture2);
	//bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_READ_FRAMEBUFFER);
	bindFramebuffer(0, viewPortResX, viewPortResY, GL_FRAMEBUFFER);
	glClearColor(0.1f, 0.1f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glDisable(GL_DEPTH_TEST);

	glUseProgram(renderSurfaceShader->getShaderId());
	renderSurfaceShader->fillUniformLocation(renderTexture);
	
	bindVertexArray(renderSurfaceVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
	
	bindVertexArray(0);
}

void Renderer::applyBloomFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture)
{
	for (int i = 0; i < 2; i++)
	{
		bool horizontal;
		GLuint textureToRender;
		if (i == 0)
		{
			bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_FRAMEBUFFER);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, targetTexture, 0);	//render into texture2
			horizontal = true;
			textureToRender = sourceTexture;		//render from texture1
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
