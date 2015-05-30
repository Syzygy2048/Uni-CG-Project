#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <map>
#include <iostream>

#include "../Shader/ShaderProgram.h"
#include "../Texture.h"

class ShaderProgram;

class Text
{
public:
	Text(const char* text, const MeshLoadInfo::LoadInfo* meshLoadInfo);
	~Text();

	void prepareText(GLfloat x, GLfloat y, GLfloat scale);
	GLuint getVAO();
	int getVerticesSize();
	Texture* getTexture();
	void draw(glm::mat4 projectionMatrix);
	glm::mat4 getProjectionMatrix();
	ShaderProgram* getShaderProgram();
	glm::vec3 getColor();
	GLfloat getX();
	GLfloat getY();
	GLfloat getScale();
	std::string getText();

	struct Character {
		GLuint TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		GLuint Advance;
	};

private:

	const MeshLoadInfo::LoadInfo* loadInfo;
	const char * text;
	/*FT_Library ft;
	FT_Face face;*/
	Texture* texture;
	ShaderProgram* shaderProgram;
	glm::mat4 projectionMatrix;
	glm::vec3 color;
	GLfloat xCoord;
	GLfloat yCoord;
	GLfloat scale;
	
	//Texture* texture;
	std::map<GLchar, Character> Characters;


	int vertexAttribPointer;
	int textureAttribPointer;

	GLuint vao;
	GLuint vertexBufferID;
	GLuint UVBufferID;
	GLuint shaderID;
	GLuint uniformID;

	int verticesSize;
};

