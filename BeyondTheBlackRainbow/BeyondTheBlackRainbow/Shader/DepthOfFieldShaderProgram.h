#pragma once

#include "ShaderProgram.h"
class DepthOfFieldShaderProgram : public ShaderProgram
{
public:
	DepthOfFieldShaderProgram(GLuint shaderProgramId);
	~DepthOfFieldShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights){};
	virtual void fillUniformLocation(Text* text){};
	virtual void fillUniformLocation(GLuint renderTexture, GLuint blurredTexture, GLuint depthBuffer);
private:
	GLuint locationTexture;
	GLuint locationBlurredTexture;
	GLuint locationDepthBuffer;
	GLuint locationFocusDistance;
};