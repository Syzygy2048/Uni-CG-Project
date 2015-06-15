#include "ShadowMappingShaderProgram.h"
#include "..\Util\UuidHelper.h"
#include "..\SceneGraph\DirectionalLightNode.h"
#include <sstream>


ShadowMappingShaderProgram::ShadowMappingShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


ShadowMappingShaderProgram::~ShadowMappingShaderProgram()
{
}

void ShadowMappingShaderProgram::loadUniformLocations()
{
	locationMVP = glGetUniformLocation(programId, "MVP");
	locationM = glGetUniformLocation(programId, "model");
	locationReverseNormals = glGetUniformLocation(programId, "reverse_normals");
	locationViewPos = glGetUniformLocation(programId, "viewPos");
	locationLightPosition = glGetUniformLocation(programId, "lightPos");
	locationFarPlane = glGetUniformLocation(programId, "far_plane");
	locationDepthMap = glGetUniformLocation(programId, "depthMap");
	locationDiffuseTexture = glGetUniformLocation(programId, "diffuseTexture");
	locationShadows = glGetUniformLocation(programId, "shadows");
}

void ShadowMappingShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 MVP = node->getModelViewProjectionMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
	glm::mat4 M = node->getModelMatrix();
	glUniformMatrix4fv(locationM, 1, GL_FALSE, &M[0][0]);
	float farPlane = node->getFramebuffer()->getFarPlane();
	glUniform1f(locationFarPlane, farPlane);
	glm::vec3 viewPos = node->getPlayerPosition();
	glUniform3fv(locationViewPos, 1, &viewPos[0]);

	glUniform1i(locationShadows, true);
	glUniform1i(locationReverseNormals, true);
	
	this->useLights(lights);
	this->bindTextures(node);
}

void ShadowMappingShaderProgram::bindTextures(MeshNode* node)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
	glUniform1i(locationDiffuseTexture, 0);
	
	glActiveTexture(GL_TEXTURE29);
	glBindTexture(GL_TEXTURE_CUBE_MAP, node->getFramebuffer()->getTexture()->getCubeMapID());
	glUniform1i(locationDepthMap, 29);
}

void ShadowMappingShaderProgram::useLights(std::vector<LightNode*> lights)
{
	for (LightNode* light : lights) {
		if (light->getLightType() == POINT_LIGHT) {
			glUniform3fv(locationLightPosition, 1, &(light->getPosition()[0]));
			return;
		}
	}
}

void ShadowMappingShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{

}

void ShadowMappingShaderProgram::fillUniformLocation(Text* text)
{

}

