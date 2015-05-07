#include "LightingShaderProgram.h"


LightingShaderProgram::LightingShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


LightingShaderProgram::~LightingShaderProgram()
{
}

void LightingShaderProgram::loadUniformLocations()
{
	locationMVP = glGetUniformLocation(shaderID, "MVP");
	locationMVP = glGetUniformLocation(shaderID, "MV");
	locationV = glGetUniformLocation(shaderID, "V");
	locationM = glGetUniformLocation(shaderID, "M");
	locationLightInvDirection_worldspace = glGetUniformLocation(shaderID, "LightInvDirection_worldspace");
	locationDepthBiasMVP = glGetUniformLocation(shaderID, "DepthBiasMVP");

}

GLuint LightingShaderProgram::getLocationMVP()
{
	return locationMVP;
}

GLuint LightingShaderProgram::getLocationTexture()
{
	return -1;
}

void LightingShaderProgram::fillUniformLocation(MeshNode* node)
{
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &node->getModelViewProjectionMatrix()[0][0]);
	//glUniformMatrix4fv(locationMVP, 1, GL_FALSE, node->getMV());
	glUniformMatrix4fv(locationV, 1, GL_FALSE, &node->getViewMatrix()[0][0]);
	glUniformMatrix4fv(locationM, 1, GL_FALSE, &glm::mat4(node->propagateMatrix())[0][0]);
	//glUniform3fv(locationLightInvDirection_worldspace, 1, GL_FALSE, node->getLightDirection());
	//glUniformMatrix4fv(locationDepthBiasMVP, 1, GL_FALSE, node->getDepthBiasMVP());
}