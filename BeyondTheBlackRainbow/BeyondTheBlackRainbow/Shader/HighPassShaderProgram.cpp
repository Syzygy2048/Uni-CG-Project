#include "HighPassShaderProgram.h"


HighPassShaderProgram::HighPassShaderProgram(GLuint shaderProgramId) : ShaderProgram(shaderProgramId)
{
}


HighPassShaderProgram::~HighPassShaderProgram()
{
}


void HighPassShaderProgram::loadUniformLocations()
{
	locationTexture = glGetUniformLocation(programId, "renderedTexture");
}

void HighPassShaderProgram::fillUniformLocation(GLuint renderTexture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glUniform1i(locationTexture, 0);
}
