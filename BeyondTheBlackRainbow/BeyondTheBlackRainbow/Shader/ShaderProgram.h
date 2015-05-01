#pragma once

#include <GL\glew.h>
#include "..\SceneGraph\MeshNode.h"


class ShaderProgram
{
public:
	
	ShaderProgram(GLuint shaderProgramID);

	GLuint getShaderId();

	ShaderProgram();
	~ShaderProgram();

	virtual void loadUniformLocations() = 0;
	virtual void fillUniformLocation(MeshNode* node) = 0;

private:
	GLuint programId;
	
	
	
};

