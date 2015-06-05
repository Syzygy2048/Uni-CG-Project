#include "Text.h"

#include <vector>

#include "..\Render\Renderer.h"
#include "..\Importers\ShaderImporter.h"



Text::Text(const char* text, const MeshLoadInfo::LoadInfo* meshLoadInfo)
{
	loadInfo = meshLoadInfo;
	this->text = text;
}


Text::~Text()
{
	//TODO
}

void Text::prepareText(GLfloat x, GLfloat y, GLfloat scale)
{
	this->x = x;
	this->y = y;
	this->scale = scale;
	Renderer* renderer = Renderer::getInstance();
	renderer->generateVertexArray(&vao);
	renderer->generateBufferObject(&vertexBufferID);
	renderer->generateBufferObject(&UVBufferID);

	vertexAttribPointer = 0;
	textureAttribPointer = 1;

	renderer->bindVertexArray(vao);
	
	unsigned int length = strlen(text);
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UV;
	for (unsigned int i = 0; i<length; i++){

		glm::vec2 vertex_up_left = glm::vec2(x + i*scale, y + scale);
		glm::vec2 vertex_up_right = glm::vec2(x + i*scale + scale, y + scale);
		glm::vec2 vertex_down_right = glm::vec2(x + i*scale + scale, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i*scale, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UV.push_back(uv_up_left);
		UV.push_back(uv_down_left);
		UV.push_back(uv_up_right);

		UV.push_back(uv_down_right);
		UV.push_back(uv_up_right);
		UV.push_back(uv_down_left);
	}
	verticesSize = vertices.size();
	renderer->fillBuffer(vertexBufferID, GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	renderer->setVertexAttribPointer(vertexAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	renderer->fillBuffer(UVBufferID, GL_ARRAY_BUFFER, UV.size() * sizeof(glm::vec2), &UV[0], GL_STATIC_DRAW);
	renderer->setVertexAttribPointer(textureAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	shaderProgram = ShaderImporter::getInstance()->loadShaderProgram(loadInfo->shaderInfo);
	shaderProgram->loadUniformLocations();
	texture = new Texture((loadInfo->texturePath).c_str());

	renderer->bindVertexArray(0);
	renderer->bindBuffer(GL_ARRAY_BUFFER, 0);

}

void Text::draw()
{
	Renderer::getInstance()->drawText(this);
}

GLuint Text::getVAO()
{
	return vao;
}

int Text::getVerticesSize()
{
	return verticesSize;
}

Texture* Text::getTexture()
{
	return texture;
}

ShaderProgram* Text::getShaderProgram()
{
	return shaderProgram;
}

std::string Text::getText()
{
	return text;
}

void Text::setText(char* text){
	this->text = text;

	Renderer* renderer = Renderer::getInstance();

	unsigned int length = strlen(text);
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UV;
	for (unsigned int i = 0; i<length; i++){

		glm::vec2 vertex_up_left = glm::vec2(x + i*scale, y + scale);
		glm::vec2 vertex_up_right = glm::vec2(x + i*scale + scale, y + scale);
		glm::vec2 vertex_down_right = glm::vec2(x + i*scale + scale, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i*scale, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UV.push_back(uv_up_left);
		UV.push_back(uv_down_left);
		UV.push_back(uv_up_right);

		UV.push_back(uv_down_right);
		UV.push_back(uv_up_right);
		UV.push_back(uv_down_left);
	}
	verticesSize = vertices.size();
	renderer->fillBuffer(vertexBufferID, GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	renderer->setVertexAttribPointer(vertexAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	renderer->fillBuffer(UVBufferID, GL_ARRAY_BUFFER, UV.size() * sizeof(glm::vec2), &UV[0], GL_STATIC_DRAW);
	renderer->setVertexAttribPointer(textureAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

}

