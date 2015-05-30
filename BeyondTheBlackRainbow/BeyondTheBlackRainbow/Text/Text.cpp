#include "Text.h"

#include <vector>

#include "..\Render\Renderer.h"
#include "..\Importers\ShaderImporter.h"



Text::Text(std::string text, const MeshLoadInfo::LoadInfo* meshLoadInfo)
{
	loadInfo = meshLoadInfo;
	this->text = text;

	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR:FreeType could not init the Library" << std::endl;
	}
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR:FreeType could not load the font" << std::endl;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	/*shaderID = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
	texture = new Texture("Holstein.DDS");
	uniformID = glGetUniformLocation(shaderID, "myTextureSampler");*/
}


Text::~Text()
{
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Text::prepareText()
{
	Renderer* renderer = Renderer::getInstance();


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

	shaderProgram = ShaderImporter::getInstance()->loadShaderProgram(loadInfo->shaderInfo);
	
}

void Text::draw(glm::mat4 projectionMatrix, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	Renderer* renderer = Renderer::getInstance();
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
	}
}

GLuint Text::getVAO()
{
	return vao;
}

int Text::getVerticesSize()
{
	return verticesSize;
}

GLuint Text::getTexture()
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

