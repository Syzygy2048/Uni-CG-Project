#pragma once

#include "ShaderProgram.h"

class HighPassShaderProgram : public ShaderProgram
{
public:
	HighPassShaderProgram(GLuint shaderProgramId);
	~HighPassShaderProgram();
	
	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights){};
	virtual void fillUniformLocation(Text* text){};
	virtual void fillUniformLocation(GLuint renderTexture);
	
private:
	GLuint locationTexture;
};

