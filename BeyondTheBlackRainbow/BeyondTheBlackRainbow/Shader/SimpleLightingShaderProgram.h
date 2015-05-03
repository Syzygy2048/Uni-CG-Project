#pragma once

#include "..\Shader\ShaderProgram.h"
#include "..\Texture.h"

class SimpleLightingShaderProgram : public ShaderProgram
{
public:
	SimpleLightingShaderProgram(GLuint shaderProgramID);
	~SimpleLightingShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node);

private:
	GLuint locationMVP;
	GLuint locationV;
	GLuint locationM;
	GLuint locationTexture;
	GLuint locationLight;
	
};

