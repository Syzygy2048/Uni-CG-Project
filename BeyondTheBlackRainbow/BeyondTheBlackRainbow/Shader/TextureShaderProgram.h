#pragma once

#include "..\Shader\ShaderProgram.h"
#include "..\Texture.h"

class TextureShaderProgram : public ShaderProgram
{
public:
	TextureShaderProgram();
	~TextureShaderProgram();
	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node);

private:
	GLuint shaderID;

	GLuint locationMVP;
	GLuint locationTexture;
};

