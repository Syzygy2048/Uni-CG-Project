#include "RenderSurfaceShaderProgram.h"


RenderSurfaceShaderProgram::RenderSurfaceShaderProgram(GLuint shaderProgramId) : ShaderProgram(shaderProgramId)
{
}


RenderSurfaceShaderProgram::~RenderSurfaceShaderProgram()
{
}


void RenderSurfaceShaderProgram::loadUniformLocations()
{
	locationTexture = glGetUniformLocation(programId, "renderedTexture");
}

void RenderSurfaceShaderProgram::fillUniformLocation(GLuint renderTexture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glUniform1i(locationTexture, 0);
}