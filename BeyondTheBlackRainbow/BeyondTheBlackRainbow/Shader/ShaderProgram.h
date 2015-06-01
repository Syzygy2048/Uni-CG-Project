#pragma once

#include <GL\glew.h>

//forward declaration
#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/LightNode.h"
class MeshNode;



class ShaderProgram
{
public:
	
	ShaderProgram(GLuint shaderProgramID);

	GLuint getShaderId();

	ShaderProgram();
	~ShaderProgram();

	virtual void loadUniformLocations() = 0;
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights) = 0;

protected:
	GLuint programId;
};

