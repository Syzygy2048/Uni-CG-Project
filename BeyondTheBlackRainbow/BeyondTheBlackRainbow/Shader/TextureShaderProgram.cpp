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

void TextureShaderProgram::fillUniformLocation(MeshNode* node)
{
	glm::mat4 MVP = node->getModelViewProjectionMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(locationTexture, GL_TEXTURE0);	
}