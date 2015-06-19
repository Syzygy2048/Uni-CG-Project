#pragma once

#include "..\Shader\ShaderProgram.h"
#include "..\Text\Text.h"

class TextShaderProgram : public ShaderProgram
{
public:
	TextShaderProgram(GLuint shaderProgramID);
	~TextShaderProgram();
	
	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights);
	virtual void fillUniformLocation(Text* text);
	virtual void fillUniformLocation(Framebuffer* frameBuffer);

private:

	GLuint locationTexture;
	GLuint locationProjection;
	GLuint locationColor;
};

