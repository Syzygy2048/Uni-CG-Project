#include "ShadowMappingShaderProgram.h"
#include "..\Util\UuidHelper.h"
#include "..\SceneGraph\DirectionalLightNode.h"


ShadowMappingShaderProgram::ShadowMappingShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


ShadowMappingShaderProgram::~ShadowMappingShaderProgram()
{
}

void ShadowMappingShaderProgram::loadUniformLocations()
{
	locationMVP = glGetUniformLocation(programId, "MVP");
	auto num = glGetUniformLocation(programId, "MV");
	//locationMV = glGetUniformLocation(programId, "MV");
	locationV = glGetUniformLocation(programId, "V");
	locationM = glGetUniformLocation(programId, "M");
	locationLightDirection = glGetUniformLocation(programId, "lightDirection");
	auto num2 = glGetUniformLocation(programId, "lightPosition");
	//locationLightPosition = glGetUniformLocation(programId, "lightPosition");
	locationDepthBiasMVP = glGetUniformLocation(programId, "DepthBiasMVP");
	locationShadowMap = glGetUniformLocation(programId, "shadowMap");
	locationTexture = glGetUniformLocation(programId, "myTextureSampler");
}

void ShadowMappingShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 MVP = node->getModelViewProjectionMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
	glm::mat4 M = node->getModelMatrix();
	glUniformMatrix4fv(locationM, 1, GL_FALSE, &M[0][0]);
	glm::mat4 V = node->getViewMatrix();
	glUniformMatrix4fv(locationV, 1, GL_FALSE, &V[0][0]);
	glm::mat4 MV = V * M;
	glUniformMatrix4fv(locationMV, 1, GL_FALSE, &MV[0][0]);
	LightNode* light = this->useLights(lights);
	glUniform4f(locationLightDirection, light->getDirection().x, light->getDirection().y, light->getDirection().z, 1.0f);
	glUniform3f(locationLightPosition, light->getPosition().x, light->getPosition().y, light->getPosition().z);
	glm::mat4 depthBiasMVP = node->getDepthBiasMatrix();
	glUniformMatrix4fv(locationDepthBiasMVP, 1, GL_FALSE, &depthBiasMVP[0][0]);
	this->bindTextures(node);
}

void ShadowMappingShaderProgram::bindTextures(MeshNode* node)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
	glUniform1i(locationTexture, 0);
	
	glActiveTexture(GL_TEXTURE29);
	glBindTexture(GL_TEXTURE_2D, node->getShadowMap()->getTextureID());
	glUniform1i(locationShadowMap, 29);
}

LightNode* ShadowMappingShaderProgram::useLights(std::vector<LightNode*> lights)
{
	for (int i = 0; i < lights.size(); i++) {
		if (lights.at(i)->getLightType() == DIRECTIONAL_LIGHT) {
			return lights.at(i);
		}
	}
	return new DirectionalLightNode(generateUuid(), glm::vec3(0, 2, 0), 10.0f, glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), LightType::DIRECTIONAL_LIGHT);
}

void ShadowMappingShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{

}

void ShadowMappingShaderProgram::fillUniformLocation(Text* text)
{

}

