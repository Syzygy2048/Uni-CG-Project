#include "Texture.h"


Texture::Texture(const std::string path)
{
	/*glGenTextures(1, &handle);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);

	unsigned int width, height;*/
	ILboolean success;

	// init DevIL.only once(!!)
	ilInit();
	// generate an image name
	ilGenImages(1, &imageID);
	//bind it
	ilBindImage(imageID);
	// match image origin to OpenGL’s
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	//load
	success = ilLoadImage((ILstring)path.c_str());
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	if (!success) {
		ilDeleteImages(1, &imageID);
	}


	/* Create and loaad texture to OpenGL */

	unsigned char * data = ilGetData();
	unsigned width, height;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


Texture::~Texture()
{
	ilDeleteImages(1, &handle);
}

void Texture::bind(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, handle);
}

int Texture::getImageID()
{
	return imageID;
}


void Texture::setHandle(GLuint textureID)
{
	handle = textureID;
}