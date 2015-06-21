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
	locationBloomTexture = glGetUniformLocation(programId, "bloomTexture");
	locationHorizontal = glGetUniformLocation(programId, "horizontal");
	locationPixelSize = glGetUniformLocation(programId, "pixelSize");
}

void BloomShaderProgram::fillUniformLocation(GLuint renderTexture, GLuint bloomTexture)
{
	glActiveTexture(GL_TEXTURE0);	
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glUniform1i(locationTexture, 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);
	glUniform1i(locationBloomTexture, 1);
}