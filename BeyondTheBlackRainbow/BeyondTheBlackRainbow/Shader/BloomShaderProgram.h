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
	virtual void fillUniformLocation(Framebuffer* frameBuffer){};
	virtual void fillUniformLocation(GLuint renderTexture, GLuint bloomTexture);
private:
	GLuint locationTexture;
	GLuint locationHorizontal;
	GLuint locationPixelSize;
	GLuint locationBloomTexture;
};