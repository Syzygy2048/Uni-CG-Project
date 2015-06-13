#include "DepthShaderProgram.h"


DepthShaderProgram::DepthShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


DepthShaderProgram::~DepthShaderProgram()
{
}

void DepthShaderProgram::loadUniformLocations() 
{
	locationDepthMVP = glGetUniformLocation(programId, "depthMVP");
	locationMVP = glGetUniformLocation(programId, "MVP");
}

void DepthShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 MVP = node->getModelMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
}

void DepthShaderProgram::fillUniformLocation(Text* text)
{
}

void DepthShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{
	glm::mat4 depthMVP = frameBuffer->getDepthMVP();
	glUniformMatrix4fv(locationDepthMVP, 1, GL_FALSE, &depthMVP[0][0]);
}
