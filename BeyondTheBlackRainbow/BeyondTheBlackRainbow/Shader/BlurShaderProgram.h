#pragma once

#include "ShaderProgram.h"

class BlurShaderProgram : public ShaderProgram
{
public:
	BlurShaderProgram(GLuint shaderProgramId);
	~BlurShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights){};
	virtual void fillUniformLocation(Text* text){};
	virtual void fillUniformLocation(GLuint renderTexture, bool horizontal, int viewPortResX, int viewPortResY);
private:
	GLuint locationTexture;
	GLuint locationHorizontal;
	GLuint locationPixelSize;
};