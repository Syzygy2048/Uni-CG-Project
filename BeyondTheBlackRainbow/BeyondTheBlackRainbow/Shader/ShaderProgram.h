#pragma once

#include <GL\glew.h>
//#include "..\SceneGraph\MeshNode.h"


class ShaderProgram
{
public:
	
	ShaderProgram(GLuint shaderProgramID);

	GLuint getShaderId();

	ShaderProgram();
	~ShaderProgram();

	virtual void loadUniformLocations() = 0;
//	virtual void fillUniformLocation(MeshNode* node) = 0;

	virtual GLuint getLocationMVP() = 0;
	virtual GLuint getLocationTexture() = 0;

private:
	GLuint programId;
	
	
	
};

