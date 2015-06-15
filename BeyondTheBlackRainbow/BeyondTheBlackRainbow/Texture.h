#pragma once

#include <string>
#include <GL\glew.h>



class Texture
{
public:

	Texture(const char* path);
	Texture(int width, int height);
	
	~Texture();

	GLuint getTextureID();
	GLuint getCubeMapID();

private:

	GLubyte* texture;
	GLuint textureID;
	GLuint cubeMapID;
	int width;
	int height;
};

