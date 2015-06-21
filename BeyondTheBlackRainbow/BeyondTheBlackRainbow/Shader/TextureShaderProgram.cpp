#include "TextureShaderProgram.h"

#include <iostream>
#include <glm\glm.hpp>


TextureShaderProgram::TextureShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


TextureShaderProgram::~TextureShaderProgram()
{
}


void TextureShaderProgram::loadUniformLocations()
{
	locationMVP = glGetUniformLocation(programId, "MVP");
	locationTexture = glGetUniformLocation(programId, "myTextureSampler");
}

void TextureShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 MVP = node->getModelViewProjectionMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
	if (node->getLoadInfo() == MeshLoadInfo::DUCK) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 0);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::ROOM) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 0);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::TABLE) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 0);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::BED) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 0);
	}
	else if (node->getLoadInfo() == MeshLoadInfo::DOOR) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->getTexture()->getTextureID());
		glUniform1i(locationTexture, 0);
	}
}
void TextureShaderProgram::fillUniformLocation(Text* text)
{

}

void TextureShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{

}
