#include "SimpleLightingShaderProgram.h"

#include <iostream>
#include <glm\glm.hpp>

SimpleLightingShaderProgram::SimpleLightingShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


SimpleLightingShaderProgram::~SimpleLightingShaderProgram()
{
}

void SimpleLightingShaderProgram::loadUniformLocations()
{
	locationMVP = glGetUniformLocation(programId, "MVP");
	locationTexture = glGetUniformLocation(programId, "myTextureSampler");
	locationV = glGetUniformLocation(programId, "V");
	locationM = glGetUniformLocation(programId, "M");
	locationLight = glGetUniformLocation(programId, "LightPosition_worldspace");

}

void SimpleLightingShaderProgram::fillUniformLocation(MeshNode* node)
{
	glm::mat4 MVP = node->getModelViewProjectionMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(locationTexture, GL_TEXTURE0);
	glm::mat4 M = node->getModelMatrix();
	glUniformMatrix4fv(locationM, 1, GL_FALSE, &M[0][0]);
	glm::mat4 V = node->getViewMatrix();
	glUniformMatrix4fv(locationV, 1, GL_FALSE, &V[0][0]);
	glm::vec3 lightPos = glm::vec3(4, 4, 4);
	glUniform3f(locationLight, lightPos.x, lightPos.y, lightPos.z);

}