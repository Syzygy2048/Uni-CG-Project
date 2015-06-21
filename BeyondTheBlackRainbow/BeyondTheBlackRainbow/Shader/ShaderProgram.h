#pragma once

#include <GL/glew.h>

//forward declaration
#include "../SceneGraph/MeshNode.h"
#include "../SceneGraph/LightNode/LightNode.h"
#include "../Text/Text.h"
#include "../Framebuffer.h"

class MeshNode;
class Text;
class Framebuffer;


class ShaderProgram
{
public:
	
	ShaderProgram(GLuint shaderProgramID);

	GLuint getShaderId();

	ShaderProgram();
	~ShaderProgram();

	virtual void loadUniformLocations() = 0;
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights) = 0;
	virtual void fillUniformLocation(Text* text) = 0;
	virtual void fillUniformLocation(Framebuffer* frameBuffer) = 0;

protected:
	GLuint programId;
};

