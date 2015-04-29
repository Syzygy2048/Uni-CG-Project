#pragma once

#include "..\Shader\ShaderProgram.h"

class LightingShaderProgram : public ShaderProgram
{
public:
	LightingShaderProgram(GLuint shaderProgramID);
	~LightingShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node);
	
private:
	GLuint shaderID;

	GLuint locationMVP;
	GLuint locationV;
	GLuint locationM;
	GLuint locationLightInvDirection_worldspace;
	GLuint locationDepthBiasMVP;
	

};

