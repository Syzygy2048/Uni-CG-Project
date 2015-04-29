#include "TextureShaderProgram.h"


TextureShaderProgram::TextureShaderProgram()
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

void TextureShaderProgram::fillUniformLocation(MeshNode* node)
{
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, node->getMVP());
	glUniform1i(locationTexture, GL_TEXTURE0);
	
}