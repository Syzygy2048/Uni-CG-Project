#pragma once

#include <string>
#include <GL\glew.h>



class Texture
{
public:

	Texture(const char* path);
	Texture();
	~Texture();

	GLuint getTextureID();

private:

	GLubyte* texture;
	GLuint textureID;
};

