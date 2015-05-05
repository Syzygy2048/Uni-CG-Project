#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>

#include "..\Texture.h"

class Text
{
public:
	Text();
	~Text();

	void prepareText(const char* text, int length, int x, int y);
	GLuint getVAO();
	int getVerticesSize();

private:
	Texture* texture;


	int vertexAttribPointer;
	int textureAttribPointer;

	GLuint vao;
	GLuint vertexBufferID;
	GLuint UVBufferID;
	GLuint shaderID;
	GLuint uniformID;

	int verticesSize;
};

