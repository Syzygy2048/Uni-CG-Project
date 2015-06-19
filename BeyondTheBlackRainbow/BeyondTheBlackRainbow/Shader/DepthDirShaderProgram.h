#pragma once

#include "..\Shader\ShaderProgram.h"

class DepthDirShaderProgram : public ShaderProgram
{
public:
	DepthDirShaderProgram(GLuint shaderProgramID);
	~DepthDirShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights);
	virtual void fillUniformLocation(Text* text);
	virtual void fillUniformLocation(Framebuffer* frameBuffer);

private:
	GLuint locationDepthMVP;
	GLuint locationMVP;
};

