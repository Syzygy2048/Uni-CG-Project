#include "TextureShaderProgram.h"
#include <iostream>


TextureShaderProgram::TextureShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


TextureShaderProgram::~TextureShaderProgram()
{
}


void TextureShaderProgram::loadUniformLocations()
{
	locationMVP = glGetUniformLocation(shaderID, "MVP");
	locationTexture = glGetUniformLocation(shaderID, "myTextureSampler");
}

GLuint TextureShaderProgram::getLocationMVP()
{
	std::cout << "getLocaonMVP" << std::endl;
	return locationMVP;
}

GLuint TextureShaderProgram::getLocationTexture()
{
	std::cout << "getLocaonTexture" << std::endl;
	return locationTexture;
}
//void TextureShaderProgram::fillUniformLocation(MeshNode* node)
//{
//	glm::mat4 MVP = node->getModelViewProjectionMatrix();
//	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
//	glUniform1i(locationTexture, GL_TEXTURE0);
//	
//}