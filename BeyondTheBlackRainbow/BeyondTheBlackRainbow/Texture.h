#pragma once

#include <string>
#include <GL\glew.h>



class Texture
{
public:

	Texture(const char* path);
	~Texture();

	void bind(int unit);

	GLuint getTextureID();

private:

	GLubyte* texture;
	GLuint textureID;
};

