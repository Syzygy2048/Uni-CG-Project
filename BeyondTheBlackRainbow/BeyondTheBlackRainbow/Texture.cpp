#include "Texture.h"
#include <iostream>
#include <FreeImage.h>


Texture::Texture(const char* path)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(path, 0);
	std::cout << path << std::endl;
	FIBITMAP* image = FreeImage_Load(formato, path);
	

	if (!image) {
		std::cout << "errorT_T" << std::endl;
	}

	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);
	std::cout << "The size of the image is: " << path << " is " << w << "*" << h << std::endl; //Some debugging code

	texture = new GLubyte[4 * w*h];
	char* pixeles = (char*)FreeImage_GetBits(image);
	//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).

	for (int j = 0; j<w*h; j++){
		texture[j * 4 + 0] = pixeles[j * 4 + 2];
		texture[j * 4 + 1] = pixeles[j * 4 + 1];
		texture[j * 4 + 2] = pixeles[j * 4 + 0];
		texture[j * 4 + 3] = pixeles[j * 4 + 3];
		//cout<<j<<": "<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<endl;
	}

	//Now generate the OpenGL texture object 

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*GLenum huboError = glGetError();
	if (huboError){

		std::cout << "There was an error loading the texture"  << huboError << std::endl;
	}*/

}


Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::bind(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint Texture::getTextureID()
{
	return textureID;
}