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
	if (node->getLoadInfo() == MeshLoadInfo::DUCK) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 0);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::ROOM) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 1);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::TABLE) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 2);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::BED) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 3);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::DOOR) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 4);
	}
	glm::mat4 M = node->getModelMatrix();
	glUniformMatrix4fv(locationM, 1, GL_FALSE, &M[0][0]);
	glm::mat4 V = node->getViewMatrix();
	glUniformMatrix4fv(locationV, 1, GL_FALSE, &V[0][0]);
	glm::vec3 lightPos = glm::vec3(0, 0, 0);
	glUniform3f(locationLight, lightPos.x, lightPos.y, lightPos.z);

}