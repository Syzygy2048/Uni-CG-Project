#pragma once

#include <string>
#include <GL\glew.h>



class Texture
{
public:

	Texture(const char* path);
	~Texture();

	void bind(int unit);

private:

	GLubyte* texture;
	GLuint textureID;
};

