#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <map>
#include <iostream>

#include "../Shader/ShaderProgram.h"
#include "../Texture/Texture.h"

class ShaderProgram;

class Text
{
public:
	Text(const char* text, const MeshLoadInfo::LoadInfo* meshLoadInfo);
	~Text();

	void prepareText(GLfloat x, GLfloat y, GLfloat scale);
	void draw(bool enableBlend);

	GLuint getVAO();
	int getVerticesSize();
	Texture* getTexture();
	ShaderProgram* getShaderProgram();
	void setText(char* text);
	std::string getText();

	void setValid(bool valid);
	bool getValid();

private:

	const MeshLoadInfo::LoadInfo* loadInfo;
	const char* text;

	float x;
	float y;
	GLfloat scale;

	Texture* texture;
	ShaderProgram* shaderProgram;

	int vertexAttribPointer;
	int textureAttribPointer;

	GLuint vao;
	GLuint vertexBufferID;
	GLuint UVBufferID;

	int verticesSize;

	bool valid;
};

