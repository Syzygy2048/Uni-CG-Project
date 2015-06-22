#include "ShadowMappingShaderProgram.h"
#include "..\Util\UuidHelper.h"
#include "..\SceneGraph\LightNode\DirectionalLightNode.h"
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
	locationV = glGetUniformLocation(programId, "V");
	locationDepthBias = glGetUniformLocation(programId, "DepthBiasMVP");
	locationReverseNormals = glGetUniformLocation(programId, "reverse_normals");
	locationViewPos = glGetUniformLocation(programId, "viewPos");
	locationDiffuseTexture = glGetUniformLocation(programId, "diffuseTexture");
	locationShadows = glGetUniformLocation(programId, "needShadows");
	locationDirDepthMap = glGetUniformLocation(programId, "dirDepthMap");
}

void ShadowMappingShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 MVP = node->getModelViewProjectionMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
	glm::mat4 M = node->getModelMatrix();
	glUniformMatrix4fv(locationM, 1, GL_FALSE, &M[0][0]);
	glm::mat4 V = node->getViewMatrix();
	glUniformMatrix4fv(locationV, 1, GL_FALSE, &V[0][0]);
	glm::mat4 bias = node->getDepthBiasMatrix();
	glUniformMatrix4fv(locationDepthBias, 1, GL_FALSE, &bias[0][0]);
	glm::vec3 viewPos = node->getPlayerPosition();
	glUniform3fv(locationViewPos, 1, &viewPos[0]);

	glUniform1i(locationShadows, true);
	glUniform1i(locationReverseNormals, true);
	
	this->useLights(lights);
	this->bindTextures(node);
}

void ShadowMappingShaderProgram::bindTextures(MeshNode* node)
{//this works for ONE light (key)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
	glUniform1i(locationDiffuseTexture, 0);
	
	std::map<std::string, Framebuffer*> framebuffers = node->getFramebuffers();
	int count = 0;
	for (int i = 0; i < framebuffers.size(); i++) {
		std::stringstream shadowMap;
		shadowMap << "shadows[";
		shadowMap << count;
		shadowMap << "].depthMap";
		if (count == 0) {
			glActiveTexture(GL_TEXTURE29);
		}
		else if (count == 1) {
			glActiveTexture(GL_TEXTURE28);
		}
		else if (count == 2) {
			glActiveTexture(GL_TEXTURE27);
		}
		else if (count == 3) {
			glActiveTexture(GL_TEXTURE26);
		}
		std::stringstream key;
		key << "pointLight";
		key << count;
		glBindTexture(GL_TEXTURE_CUBE_MAP, framebuffers.find("pointLight0")->second->getTexture()->getCubeMapID());
		auto loc = glGetUniformLocation(programId, shadowMap.str().c_str());
		glUniform1i(glGetUniformLocation(programId, shadowMap.str().c_str()), (29 - count));
		
		std::stringstream farPlane;
		farPlane << "shadows[";
		farPlane << count;
		farPlane << "].farPlane";
		glUniform1f(glGetUniformLocation(programId, farPlane.str().c_str()), framebuffers.find(key.str())->second->getFarPlane());
		
		count++;
	}
}

void ShadowMappingShaderProgram::useLights(std::vector<LightNode*> lights)
{
	for (int i = 0; i < lights.size(); i++) {
		std::stringstream position;
		position << "lights[";
		position << i;
		position << "].position";
		//auto loc = glGetUniformLocation(programId, position.str().c_str());
		glUniform3f(glGetUniformLocation(programId, position.str().c_str()), lights.at(i)->getPosition().x, lights.at(i)->getPosition().y, lights.at(i)->getPosition().z);
		std::stringstream intensity;
		intensity << "lights[";
		intensity << i;
		intensity << "].intensity";
		//auto loc2 = glGetUniformLocation(programId, intensity.str().c_str());
		glUniform1f(glGetUniformLocation(programId, intensity.str().c_str()), lights.at(i)->getIntensity());
		std::stringstream color;
		color << "lights[";
		color << i;
		color << "].color";
		//auto loc3 = glGetUniformLocation(programId, color.str().c_str());
		glUniform3f(glGetUniformLocation(programId, color.str().c_str()), lights.at(i)->getColor().x, lights.at(i)->getColor().y, lights.at(i)->getColor().z);
		std::stringstream type;
		type << "lights[";
		type << i;
		type << "].type";
		//auto loc4 = glGetUniformLocation(programId, type.str().c_str());
		if (lights.at(i)->getLightType() == POINT_LIGHT) {
			glUniform1f(glGetUniformLocation(programId, type.str().c_str()), 1.0f);
		}
		else if (lights.at(i)->getLightType() == DIRECTIONAL_LIGHT) {
			glUniform1f(glGetUniformLocation(programId, type.str().c_str()), 2.0f);
			std::stringstream direction;
			direction << "lights[";
			direction << i;
			direction << "].direction";
			//auto loc5 = glGetUniformLocation(programId, direction.str().c_str());
			glUniform3f(glGetUniformLocation(programId, direction.str().c_str()), lights.at(i)->getDirection().x, lights.at(i)->getDirection().y, lights.at(i)->getDirection().z);
			//std::cout << lights.at(i)->getDirection().x << " " << lights.at(i)->getDirection().y << " " << lights.at(i)->getDirection().z << std::endl;
		}
		else if (lights.at(i)->getLightType() == SPOT_LIGHT) {
			glUniform1f(glGetUniformLocation(programId, type.str().c_str()), 3.0f);
			std::stringstream direction;
			direction << "lights[";
			direction << i;
			direction << "].direction";
			//auto loc5 = glGetUniformLocation(programId, direction.str().c_str());
			glUniform3f(glGetUniformLocation(programId, direction.str().c_str()), lights.at(i)->getDirection().x, lights.at(i)->getDirection().y, lights.at(i)->getDirection().z);
			std::stringstream cutOff;
			cutOff << "lights[";
			cutOff << i;
			cutOff << "].cutOff";
			glUniform2f(glGetUniformLocation(programId, cutOff.str().c_str()), lights.at(i)->getCutOff().x, lights.at(i)->getCutOff().y);
		}
	}
}

void ShadowMappingShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{

}

void ShadowMappingShaderProgram::fillUniformLocation(Text* text)
{

}

