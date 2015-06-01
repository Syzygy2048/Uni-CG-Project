#include "SimpleLightingShaderProgram.h"

#include <iostream>
#include <sstream>
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
	//locationLight = glGetUniformLocation(programId, "LightPosition_worldspace");

}

void SimpleLightingShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	this->useLights(lights);
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
	//glm::vec3 lightPos = glm::vec3(2, 1.5, -2.5);
	/*glm::vec3 lightPos = light->getPosition();
	glUniform3f(locationLight, lightPos.x, lightPos.y, lightPos.z);*/

}

void SimpleLightingShaderProgram::useLights(std::vector<LightNode*> lights)
{
	/*if (lights.size() >= 1) {
		glUniform3f(glGetUniformLocation(programId, "pointLights[0].position"), lights.at(0)->getPosition().x, lights.at(0)->getPosition().y, lights.at(0)->getPosition().z);
		glUniform3f(glGetUniformLocation(programId, "pointLights[0].intensity"), lights.at(0)->getIntensity().x, lights.at(0)->getIntensity().y, lights.at(0)->getIntensity().z);
		glUniform3f(glGetUniformLocation(programId, "pointLights[0].color"), lights.at(0)->getColor().x, lights.at(0)->getColor().y, lights.at(0)->getColor().z);
	}*/

	glUniform1f(glGetUniformLocation(programId, "sumLights"), lights.size());
	for (int i = 0; i < lights.size(); i++) {
		std::stringstream position;
		position << "pointLights[";
		position << i;
		position << "].position";
	//auto loc = glGetUniformLocation(programId, position.str().c_str());
	glUniform3f(glGetUniformLocation(programId, position.str().c_str()), lights.at(i)->getPosition().x, lights.at(i)->getPosition().y, lights.at(i)->getPosition().z);
	std::stringstream intensity;
	intensity << "pointLights[";
	intensity << i;
	intensity << "].intensity";
	//auto loc2 = glGetUniformLocation(programId, intensity.str().c_str());
	glUniform3f(glGetUniformLocation(programId, intensity.str().c_str()), lights.at(i)->getIntensity().x, lights.at(i)->getIntensity().y, lights.at(i)->getIntensity().z);
	std::stringstream color;
	color << "pointLights[";
	color << i;
	color << "].color";
	//auto loc3 = glGetUniformLocation(programId, color.str().c_str());
	glUniform3f(glGetUniformLocation(programId, color.str().c_str()), lights.at(i)->getColor().x, lights.at(i)->getColor().y, lights.at(i)->getColor().z);
	//std::cout << "Position: " << lights.at(i)->getPosition().x << " " << lights.at(i)->getPosition().y << " " << lights.at(i)->getPosition().z << std::endl;
	}
}