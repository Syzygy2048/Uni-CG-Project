#include "Text.h"

#include <vector>
#include <cstring>

#include "shader.hpp"

#include "..\Render\Renderer.h"

Text::Text()
{
	shaderID = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
	texture = new Texture("Holstein.DDS");
	uniformID = glGetUniformLocation(shaderID, "myTextureSampler");
}


Text::~Text()
{
	// Delete buffers
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &UVBufferID);

	// Delete texture
	texture->~Texture();

	// Delete shader
	glDeleteProgram(shaderID);
}

void Text::prepareText(const char* text, int size, int x, int y)
{
	Renderer* renderer = Renderer::getInstance();
	renderer->generateVertexArray(&vao);
	renderer->generateBufferObject(&vertexBufferID);
	renderer->generateBufferObject(&UVBufferID);

	vertexAttribPointer = 10;
	textureAttribPointer = 11;

	renderer->bindVertexArray(vao);

	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i<length; i++){

		glm::vec2 vertex_up_left = glm::vec2(x + i*size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i*size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i*size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i*size, y);

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
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	renderer->fillBuffer(vertexBufferID, GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	renderer->setVertexAttribPointer(vertexAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	renderer->fillBuffer(UVBufferID, GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
	renderer->setVertexAttribPointer(textureAttribPointer, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	renderer->bindVertexArray(0);
	renderer->bindBuffer(GL_ARRAY_BUFFER, 0);
	renderer->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	/*glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);*/

	
	// Bind shader
	//glUseProgram(shaderID);

	//// Bind texture
	///*texture->bind(0);
	//glActiveTexture(GL_TEXTURE0);*/

	//glActiveTexture(GL_TEXTURE30);
	//glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	//glUniform1i(uniformID, 30);
	
	//glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(uniformID, 0);

	verticesSize = vertices.size();

	// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(4);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//// 2nd attribute buffer : UVs
	//glEnableVertexAttribArray(5);
	//glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//// Draw call
	//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//glDisable(GL_BLEND);

	/*glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);*/
}

GLuint Text::getVAO()
{
	return vao;
}

int Text::getVerticesSize()
{
	return verticesSize;
}

void Text::useShader(Text* text)
{
	glUseProgram(text->shaderID);

	// Bind texture
	/*texture->bind(0);
	glActiveTexture(GL_TEXTURE0);*/

	glActiveTexture(GL_TEXTURE30);
	glBindTexture(GL_TEXTURE_2D, text->texture->getTextureID());
	glUniform1i(uniformID, 30);
}
