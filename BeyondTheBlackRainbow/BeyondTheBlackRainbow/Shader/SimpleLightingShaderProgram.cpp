#include "SimpleLightingShaderProgram.h"

#include <iostream>
#include <sstream>
#include <glm\glm.hpp>
#include "..\SceneGraph\LightsEnum.h"

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

	/*float sumPointLights = 0;
	float sumDirLights = 0;*/
	for (int i = 0; i < lights.size(); i++) {
		//if (lights.at(i)->getLightType() == POINT_LIGHT) {
		std::stringstream position;
		position << "lights[";
		position << i;
		position << "].position";
		auto loc = glGetUniformLocation(programId, position.str().c_str());
		glUniform3f(glGetUniformLocation(programId, position.str().c_str()), lights.at(i)->getPosition().x, lights.at(i)->getPosition().y, lights.at(i)->getPosition().z);
		std::stringstream intensity;
		intensity << "lights[";
		intensity << i;
		intensity << "].intensity";
		auto loc2 = glGetUniformLocation(programId, intensity.str().c_str());
		glUniform1f(glGetUniformLocation(programId, intensity.str().c_str()), lights.at(i)->getIntensity());
		std::stringstream color;
		color << "lights[";
		color << i;
		color << "].color";
		auto loc3 = glGetUniformLocation(programId, color.str().c_str());
		glUniform3f(glGetUniformLocation(programId, color.str().c_str()), lights.at(i)->getColor().x, lights.at(i)->getColor().y, lights.at(i)->getColor().z);
		std::stringstream type;
		type << "lights[";
		type << i;
		type << "].type";
		auto loc4 = glGetUniformLocation(programId, type.str().c_str());
		if (lights.at(i)->getLightType() == POINT_LIGHT) {
			glUniform1f(glGetUniformLocation(programId, type.str().c_str()), 1.0f);
		}
		else if (lights.at(i)->getLightType() == DIRECTIONAL_LIGHT) {
			glUniform1f(glGetUniformLocation(programId, type.str().c_str()), 2.0f);
			std::stringstream direction;
			direction << "lights[";
			direction << i;
			direction << "].direction";
			auto loc5 = glGetUniformLocation(programId, direction.str().c_str());
			glUniform3f(glGetUniformLocation(programId, direction.str().c_str()), lights.at(i)->getDirection().x, lights.at(i)->getDirection().y, lights.at(i)->getDirection().z);
			//std::cout << lights.at(i)->getDirection().x << " " << lights.at(i)->getDirection().y << " " << lights.at(i)->getDirection().z << std::endl;
		}
		//std::cout << "Position: " << lights.at(i)->getPosition().x << " " << lights.at(i)->getPosition().y << " " << lights.at(i)->getPosition().z << std::endl;
		//sumPointLights++;
		//}
		//else if (lights.at(i)->getLightType() == DIRECTIONAL_LIGHT)
		//{
		//	std::stringstream position;
		//	position << "dirLights[";
		//	position << sumDirLights;
		//	position << "].position";
		//	auto loc = glGetUniformLocation(programId, position.str().c_str());
		//	glUniform3f(glGetUniformLocation(programId, position.str().c_str()), lights.at(i)->getPosition().x, lights.at(i)->getPosition().y, lights.at(i)->getPosition().z);
		//	std::stringstream intensity;
		//	intensity << "dirLights[";
		//	intensity << sumDirLights;
		//	intensity << "].intensity";
		//	auto loc2 = glGetUniformLocation(programId, intensity.str().c_str());
		//	glUniform1f(glGetUniformLocation(programId, intensity.str().c_str()), lights.at(i)->getIntensity());
		//	std::stringstream color;
		//	color << "dirLights[";
		//	color << sumDirLights;
		//	color << "].color";
		//	auto loc3 = glGetUniformLocation(programId, color.str().c_str());
		//	glUniform3f(glGetUniformLocation(programId, color.str().c_str()), lights.at(i)->getColor().x, lights.at(i)->getColor().y, lights.at(i)->getColor().z);
		//	std::stringstream direction;
		//	direction << "dirLights[";
		//	direction << sumDirLights;
		//	direction << "].direction";
		//	auto loc4 = glGetUniformLocation(programId, direction.str().c_str());
		//	glUniform3f(glGetUniformLocation(programId, direction.str().c_str()), lights.at(i)->getDirection().x, lights.at(i)->getDirection().y, lights.at(i)->getDirection().z);
		//	sumDirLights++;
		//	//std::cout << "Position (DirLight): " << lights.at(i)->getPosition().x << " " << lights.at(i)->getPosition().y << " " << lights.at(i)->getPosition().z << std::endl;
		//}
	}
}

void SimpleLightingShaderProgram::fillUniformLocation(Text* text)
{
}