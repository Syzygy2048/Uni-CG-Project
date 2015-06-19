#include "DirShadowMappingShaderProgram.h"
#include "..\Util\UuidHelper.h"
#include "..\SceneGraph\DirectionalLightNode.h"
#include <sstream>


DirShadowMappingShaderProgram::DirShadowMappingShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


DirShadowMappingShaderProgram::~DirShadowMappingShaderProgram()
{
}

void DirShadowMappingShaderProgram::loadUniformLocations()
{
	locationMVP = glGetUniformLocation(programId, "MVP");
	//auto num = glGetUniformLocation(programId, "MV");
	locationMV = glGetUniformLocation(programId, "MV");
	locationV = glGetUniformLocation(programId, "V");
	locationM = glGetUniformLocation(programId, "M");
	locationLightDirection = glGetUniformLocation(programId, "lightDirection");
	//auto num2 = glGetUniformLocation(programId, "LightPosition");
	locationLightPosition = glGetUniformLocation(programId, "lightPosition");
	locationDepthBiasMVP = glGetUniformLocation(programId, "DepthBiasMVP");
	locationShadowMap = glGetUniformLocation(programId, "shadowMap");
	locationTexture = glGetUniformLocation(programId, "myTextureSampler");
}

void DirShadowMappingShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 MVP = node->getModelViewProjectionMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
	glm::mat4 M = node->getModelMatrix();
	glUniformMatrix4fv(locationM, 1, GL_FALSE, &M[0][0]);
	glm::mat4 V = node->getViewMatrix();
	glUniformMatrix4fv(locationV, 1, GL_FALSE, &V[0][0]);
	glm::mat4 MV = V * M;
	glUniformMatrix4fv(locationMV, 1, GL_FALSE, &MV[0][0]);
	this->useLights(lights);
	glm::mat4 depthBiasMVP = node->getDepthBiasMatrix();
	glUniformMatrix4fv(locationDepthBiasMVP, 1, GL_FALSE, &depthBiasMVP[0][0]);
	this->bindTextures(node);
}

void DirShadowMappingShaderProgram::bindTextures(MeshNode* node)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
	glUniform1i(locationTexture, 0);
	
	glActiveTexture(GL_TEXTURE29);
	glBindTexture(GL_TEXTURE_2D, node->getShadowMap()->getTextureID());
	glUniform1i(locationShadowMap, 29);
}

void DirShadowMappingShaderProgram::useLights(std::vector<LightNode*> lights)
{
	for (int i = 0; i < lights.size(); i++) {
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
	}
}

void DirShadowMappingShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{

}

void DirShadowMappingShaderProgram::fillUniformLocation(Text* text)
{

}

