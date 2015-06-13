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
	auto num = locationDepthMVP;
}

void DepthShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{

}

void DepthShaderProgram::fillUniformLocation(Text* text)
{
}

void DepthShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{
	glm::mat4 depthMVP = frameBuffer->getDepthMVP();
	glUniformMatrix4fv(locationDepthMVP, 1, GL_FALSE, &depthMVP[0][0]);
}
