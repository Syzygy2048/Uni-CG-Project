#include "DepthOfFieldShaderProgram.h"

#include "../Util/DoFHelper.h"


DepthOfFieldShaderProgram::DepthOfFieldShaderProgram(GLuint shaderProgramId) : ShaderProgram(shaderProgramId)
{
}


DepthOfFieldShaderProgram::~DepthOfFieldShaderProgram()
{
}


void DepthOfFieldShaderProgram::loadUniformLocations()
{
	locationTexture = glGetUniformLocation(programId, "renderedTexture");
	locationBlurredTexture = glGetUniformLocation(programId, "blurredTexture");
	locationDepthBuffer = glGetUniformLocation(programId, "depthBuffer");
	locationFocusDistance = glGetUniformLocation(programId, "focusDistance");
	locationProjectionMatrixInverse = glGetUniformLocation(programId, "projectionMatrixInverse");
}

void DepthOfFieldShaderProgram::fillUniformLocation(GLuint renderTexture, GLuint bloomTexture, GLuint depthTexture, glm::mat4 projectionMatrixInverse)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glUniform1i(locationTexture, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);
	glUniform1i(locationBlurredTexture, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(locationDepthBuffer, 2);

	glUniformMatrix4fv(locationProjectionMatrixInverse, 1, GL_FALSE, &projectionMatrixInverse[0][0]);

	float distance = DoFHelper::getInstance()->getFocusDistance();
	distance = (distance - 0.1f) / (5.f - 0.1f);
	if (distance > 1)
		distance = 1;
	glUniform1f(locationFocusDistance, distance);
}