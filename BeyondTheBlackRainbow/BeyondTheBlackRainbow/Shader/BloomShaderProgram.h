#pragma once

#include "ShaderProgram.h"

class BloomShaderProgram : public ShaderProgram
{
public:
	BloomShaderProgram(GLuint shaderProgramId);
	~BloomShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights){};
	virtual void fillUniformLocation(Text* text){};
	virtual void fillUniformLocation(GLuint textureUnit);
private:
	GLuint locationTexture;
};