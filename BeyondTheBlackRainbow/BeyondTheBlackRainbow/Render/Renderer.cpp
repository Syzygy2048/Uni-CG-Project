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

	this->viewPortResX = viewPortResX;
	this->viewPortResY = viewPortResY;

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
	glViewport(0, 0, 2048, 2048);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::unbindFrameBuffer(GLenum bufferType)
{
	glBindFramebuffer(bufferType, 0);
	glViewport(0, 0, viewPortResX, viewPortResY);

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
	//getShadows (dirLight)
	std::vector<LightNode*> lights = this->getLights(node);

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
	glm::vec3 playerPosition = glm::vec3(glm::inverse(node->getViewMatrix())[0][3], glm::inverse(node->getViewMatrix())[1][3], glm::inverse(node->getViewMatrix())[2][3]);
	for (int i = 0; i < lights.size(); i++) {
		if (lights.at(i)->getLightType() == DIRECTIONAL_LIGHT) { 
			glm::vec3 lightInvDir = (lights.at(i)->getDirection());
			glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir + playerPosition, glm::vec3(0, 0, 0) + playerPosition, glm::vec3(0, 1, 0));

			node->setDepthBiasMVP(depthProjectionMatrix * depthViewMatrix * depthModelMatrix);
			node->setShadowMap(framebuffers.find("dirLight")->second->getTexture());
		}
	}
	return lights;
}

void Renderer::drawText(Text* text, bool enableBlend)
{
	this->useShader(text);

	bindVertexArray(text->getVAO());

	if (enableBlend) {
		glEnable(GL_BLEND);
	}
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
}

void Renderer::useShader(Framebuffer* framebuffer, MeshNode* node)
{
	ShaderProgram* shaderProgram = framebuffer->getShaderProgram();
	glUseProgram(shaderProgram->getShaderId());

	shaderProgram->fillUniformLocation(framebuffer);
	shaderProgram->fillUniformLocation(node, lights);
}


void Renderer::setFrameBuffers(std::map<std::string, Framebuffer*> framebuffers)
{
	this->framebuffers = framebuffers;
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

	glGenTextures(1, &highPassTexture);
	glBindTexture(GL_TEXTURE_2D, highPassTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewPortResX, viewPortResY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	const char * label = "High Pass";
	glObjectLabel(GL_TEXTURE, highPassTexture, 9, label);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &renderTexture2);
	glBindTexture(GL_TEXTURE_2D, renderTexture2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewPortResX, viewPortResY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	const char * label1 = "Off";
	glObjectLabel(GL_TEXTURE, renderTexture2, 3, label1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewPortResX, viewPortResY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	const char * label2 = "Main";
	glObjectLabel(GL_TEXTURE, renderTexture, 4, label2);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);



	glGenTextures(1, &dummyDepthTexture);
	glBindTexture(GL_TEXTURE_2D, dummyDepthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, viewPortResX, viewPortResY, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);//GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_NEVER);
	const char * label4 = "depth";
	glObjectLabel(GL_TEXTURE, dummyDepthTexture, 5, label4);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &renderDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, renderDepthBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, viewPortResX, viewPortResY, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);//GL_COMPARE_REF_TO_TEXTURE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_NEVER);
	const char * label3 = "dummy depth";
	glObjectLabel(GL_TEXTURE, renderDepthBuffer, 11, label3);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderDepthBuffer, 0);


	//glGenRenderbuffers(1, &dummyDepthTexture);
	//glBindRenderbuffer(GL_RENDERBUFFER, dummyDepthTexture);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewPortResX, viewPortResY);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dummyDepthTexture);
	//const char * label4 = "Dummy Depth";
	//glObjectLabel(GL_RENDERBUFFER, dummyDepthTexture, 11, label4);
	//
	//glGenRenderbuffers(1, &renderDepthBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, renderDepthBuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewPortResX, viewPortResY);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderDepthBuffer);
	//const char * label3 = "Depth";
	//glObjectLabel(GL_RENDERBUFFER, renderDepthBuffer, 5, label3);
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	

	renderSurfaceShader = (RenderSurfaceShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::RENDER_SURFACE);
	renderSurfaceShader->loadUniformLocations();
	

	highPassShader = (HighPassShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::HIGH_PASS);
	highPassShader->loadUniformLocations();
	
	bloomShader = (BloomShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::BLOOM_SHADER);
	bloomShader->loadUniformLocations();

	blurShader = (BlurShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::BLUR_SHADER);
	blurShader->loadUniformLocations();	

	dofShader = (DepthOfFieldShaderProgram*)ShaderImporter::getInstance()->loadShaderProgram(MeshLoadInfo::DOF_SHADER);
	dofShader->loadUniformLocations();
}

void Renderer::configureFramebufferForPostProcessing(int viewPortResX, int viewPortResY)
{
	bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_DRAW_FRAMEBUFFER);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderDepthBuffer, 0);
	glClearColor(0.7f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_ALPHA)
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
	//bindDepthBuffer(renderDepthBuffer, viewPortResX, viewPortResY);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderToScreen(int viewPortResX, int viewPortResY, bool enableBloom)
{
	//glBindRenderbuffer(GL_RENDERBUFFER, dummyDepthTexture);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dummyDepthTexture);
	if (enableBloom) {
		applyBloomFilter(viewPortResX, viewPortResY, renderTexture, highPassTexture);
	}
	else {
		applyDepthOfFieldFilter(viewPortResX, viewPortResY, renderTexture, renderTexture2, renderDepthBuffer);
	}

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

void Renderer::applyDepthOfFieldFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture, GLuint depthHelper)
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, highPassTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, dummyDepthTexture, 0);
	
	glUseProgram(renderSurfaceShader->getShaderId());		//copy sourcetexture into highpass texture
	renderSurfaceShader->fillUniformLocation(sourceTexture);
	bindVertexArray(renderSurfaceVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6); 
	bindVertexArray(0);
	
	applyBlurFilter(viewPortResX, viewPortResY, highPassTexture, targetTexture);		//apply blur to highpassTexture

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderDepthBuffer, 0);
	
	glUseProgram(dofShader->getShaderId());
	dofShader->fillUniformLocation(sourceTexture, highPassTexture, renderDepthBuffer);
	
	bindVertexArray(renderSurfaceVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	bindVertexArray(0);
}

void Renderer::applyBloomFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture)
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD);

	applyHighPassFilter(viewPortResX, viewPortResY, renderTexture, highPassTexture);
	applyBlurFilter(viewPortResX, viewPortResY, highPassTexture, renderTexture2);
	
	bindFramebuffer(renderFrameBuffer, viewPortResX, viewPortResY, GL_FRAMEBUFFER);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sourceTexture, 0);

	glUseProgram(bloomShader->getShaderId());
	bloomShader->fillUniformLocation(sourceTexture, targetTexture);

	bindVertexArray(renderSurfaceVAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	bindVertexArray(0);

	//glDisable(GL_BLEND);
}

void Renderer::applyBlurFilter(int viewPortResX, int viewPortResY, GLuint sourceTexture, GLuint targetTexture)
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
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sourceTexture, 0);		//render into texture
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			horizontal = false;
			textureToRender = targetTexture;	//render from texture2
		}

		glUseProgram(blurShader->getShaderId());
		blurShader->fillUniformLocation(textureToRender, horizontal, viewPortResX, viewPortResY);

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
