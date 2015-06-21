#include "BlurShaderProgram.h"


BlurShaderProgram::BlurShaderProgram(GLuint shaderProgramId) : ShaderProgram(shaderProgramId)
{
}


BlurShaderProgram::~BlurShaderProgram()
{
}


void BlurShaderProgram::loadUniformLocations()
{
	locationTexture = glGetUniformLocation(programId, "renderedTexture");
	locationHorizontal = glGetUniformLocation(programId, "horizontal");
	locationPixelSize = glGetUniformLocation(programId, "pixelSize");
}

void BlurShaderProgram::fillUniformLocation(GLuint renderTexture, bool horizontal, int viewPortResX, int viewPortResY)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glUniform1i(locationTexture, 0);
	glUniform1i(locationHorizontal, horizontal);
	glUniform2f(locationPixelSize, 1.f / viewPortResX, 1.f / viewPortResY);
}