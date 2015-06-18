#include "DepthDirShaderProgram.h"


DepthDirShaderProgram::DepthDirShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
}


DepthDirShaderProgram::~DepthDirShaderProgram()
{
}

void DepthDirShaderProgram::loadUniformLocations() 
{
	locationDepthMVP = glGetUniformLocation(programId, "depthMVP");
	locationMVP = glGetUniformLocation(programId, "MVP");
}

void DepthDirShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{
	glm::mat4 MVP = node->getModelMatrix();
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, &MVP[0][0]);
}

void DepthDirShaderProgram::fillUniformLocation(Text* text)
{
}

void DepthDirShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{
	glm::mat4 depthMVP = frameBuffer->getDepthMVP();
	glUniformMatrix4fv(locationDepthMVP, 1, GL_FALSE, &depthMVP[0][0]);
}
