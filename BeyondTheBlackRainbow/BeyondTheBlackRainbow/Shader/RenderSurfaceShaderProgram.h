#pragma once

#include "ShaderProgram.h"

class RenderSurfaceShaderProgram : public ShaderProgram
{
public:
	RenderSurfaceShaderProgram(GLuint shaderProgramId);
	~RenderSurfaceShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights){};
	virtual void fillUniformLocation(Text* text){};
	virtual void fillUniformLocation(GLuint renderTexture);
private:
	GLuint locationTexture;
};