#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

#include "../Shader/ShaderProgram.h"

class ShaderProgram;

class Text
{
public:
	Text(std::string text, const MeshLoadInfo::LoadInfo* meshLoadInfo);
	~Text();

	void prepareText();
	GLuint getVAO();
	int getVerticesSize();
	GLuint getTexture();
	void draw(glm::mat4 projectionMatrix, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
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
	std::string text;
	FT_Library ft;
	FT_Face face;
	GLuint texture;
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

