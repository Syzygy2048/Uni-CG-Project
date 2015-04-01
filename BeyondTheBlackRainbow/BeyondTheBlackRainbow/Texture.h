#pragma once

#include <string>
#include <GL\glew.h>
#include <IL\il.h>


class Texture
{
public:

	Texture(const std::string path);
	~Texture();

	void bind(int unit);
	int getImageID();
	void setHandle(GLuint textureID);

private:

	GLuint handle;
	ILuint imageID;
};

