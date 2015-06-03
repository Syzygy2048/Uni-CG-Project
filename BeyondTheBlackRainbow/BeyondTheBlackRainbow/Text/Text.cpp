#include "Text.h"

#include <vector>

#include "..\Render\Renderer.h"
#include "..\Importers\ShaderImporter.h"



Text::Text(const char* text, const MeshLoadInfo::LoadInfo* meshLoadInfo)
{
	loadInfo = meshLoadInfo;
	this->text = text;

	/*glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);*/
	
	/*shaderID = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
	texture = new Texture("Holstein.DDS");
	uniformID = glGetUniformLocation(shaderID, "myTextureSampler");*/
}


Text::~Text()
{
	
}

void Text::prepareText(GLfloat x, GLfloat y, GLfloat scale)
{
	/*Renderer* renderer = Renderer::getInstance();


	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR:FreeType failed to load Glyph" << std::endl;
			continue;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	renderer->generateVertexArray(&vao);
	renderer->generateBufferObject(&vertexBufferID);

	vertexAttribPointer = 10;

	renderer->bindVertexArray(vao);
	renderer->bindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	renderer->fillBuffer(vertexBufferID, GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	renderer->setVertexAttribPointer(vertexAttribPointer, 4, GL_FLAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	renderer->bindBuffer(GL_ARRAY_BUFFER, vertexAttribPointer);
	renderer->bindVertexArray(vertexAttribPointer);
	*/
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

void Text::draw(glm::mat4 projectionMatrix)
{
	this->projectionMatrix = projectionMatrix;
	Renderer::getInstance()->drawText(this);
	/*Renderer* renderer = Renderer::getInstance();
	this->projectionMatrix = projectionMatrix;
	this->color = color;
	xCoord = x;
	yCoord = y;
	this->scale = scale;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		renderer->bindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		renderer->buffersSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		renderer->bindBuffer(GL_ARRAY_BUFFER, 10);

		renderer->drawText(this);

		x += (ch.Advance >> 6) * scale;
	}*/
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

glm::mat4 Text::getProjectionMatrix()
{
	return projectionMatrix;
}

ShaderProgram* Text::getShaderProgram()
{
	return shaderProgram;
}

glm::vec3 Text::getColor()
{
	return color;
}

GLfloat Text::getX()
{
	return xCoord;
}

GLfloat Text::getY()
{
	return yCoord;
}

GLfloat Text::getScale()
{
	return scale;
}

std::string Text::getText()
{
	return text;
}

