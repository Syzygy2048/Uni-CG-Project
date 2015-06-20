#include "DepthShaderProgram.h"
#include <sstream>

DepthShaderProgram::DepthShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


DepthShaderProgram::~DepthShaderProgram()
{
}

void DepthShaderProgram::loadUniformLocations() 
{
	locationFarPlane = glGetUniformLocation(programId, "farPlane");
	locationLightPos = glGetUniformLocation(programId, "lightPos");
	locationModel = glGetUniformLocation(programId, "model");
}

void DepthShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 M = node->getModelMatrix();
	glUniformMatrix4fv(locationModel, 1, GL_FALSE, &M[0][0]);
}

void DepthShaderProgram::fillUniformLocation(Text* text)
{
}

void DepthShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{
	float farPlane = frameBuffer->getFarPlane();
	glUniform1f(locationFarPlane, farPlane);
	glUniform3fv(locationLightPos, 1, &frameBuffer->getMyLight()->getPosition()[0]);
	this->useGeometry(frameBuffer->getDepthTransfomrs());
}

void DepthShaderProgram::useLights(std::vector<LightNode*> lights)
{
}

void DepthShaderProgram::useGeometry(std::vector<glm::mat4> depthTransforms)
{
	for (int i = 0; i < depthTransforms.size(); i++) {
		std::stringstream locationTransforms;
		locationTransforms << "depthTransforms[";
		locationTransforms << i;
		locationTransforms << "]";
		glUniformMatrix4fv(glGetUniformLocation(programId, locationTransforms.str().c_str()), 1, GL_FALSE, &depthTransforms.at(i)[0][0]);
	}
}


