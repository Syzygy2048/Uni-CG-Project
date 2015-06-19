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
	locationHorizontal = glGetUniformLocation(programId, "horizontal");
}

void BloomShaderProgram::fillUniformLocation(bool horizontal)
{
	glUniform1i(locationTexture, 0);
	glUniform1i(locationHorizontal, horizontal);
}