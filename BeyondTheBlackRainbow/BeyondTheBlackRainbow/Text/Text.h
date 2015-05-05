#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>

#include "..\Texture.h"

class Text
{
public:
	Text(const char* text, int length, int x, int y);
	~Text();

	void drawText(const char* text, int length, int x, int y);
	const char* getText();

private:
	const char* myText;
	int mySize;
	int myX;
	int myY;
	Texture* texture;

	GLuint vertexBufferID;
	GLuint UVBufferID;
	GLuint shaderID;
	GLuint uniformID;
};

