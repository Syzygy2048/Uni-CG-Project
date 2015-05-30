#pragma once

#include "..\Shader\ShaderProgram.h"
#include "..\Text\Text.h"

class TextShaderProgram : public ShaderProgram
{
public:
	TextShaderProgram(GLuint shaderProgramID);
	~TextShaderProgram();
	
	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node);
	void fillUniformLocation(Text* text);

private:

	GLuint locationTexture;
	GLuint locationProjection;
	GLuint locationColor;
};

