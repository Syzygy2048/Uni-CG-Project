#include "BloomShaderProgram.h"


BloomShaderProgram::BloomShaderProgram(GLuint shaderProgramId) : ShaderProgram(shaderProgramId)
{
}


BloomShaderProgram::~BloomShaderProgram()
{
}


void BloomShaderProgram::loadUniformLocations()
{
	locationTexture = glGetUniformLocation(programId, "renderedTexture");
}

void BloomShaderProgram::fillUniformLocation(GLuint textureUnit)
{
	glUniform1i(programId, textureUnit);
}