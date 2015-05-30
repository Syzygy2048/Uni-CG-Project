#include "TextShaderProgram.h"


TextShaderProgram::TextShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
	
}


TextShaderProgram::~TextShaderProgram()
{
}

void TextShaderProgram::loadUniformLocations()
{
	locationTexture = glGetUniformLocation(programId, "text");
	locationProjection = glGetUniformLocation(programId, "projectionMatrix");
	locationColor = glGetUniformLocation(programId, "textColor");
}

void TextShaderProgram::fillUniformLocation(MeshNode* node)
{	
}

void TextShaderProgram::fillUniformLocation(Text* text)
{
	glUniformMatrix4fv(locationProjection, 1, GL_FALSE, &(text->getProjectionMatrix())[0][0]);
	glUniform3f(locationColor, text->getColor().x, text->getColor().y, text->getColor().z);
	glActiveTexture(GL_TEXTURE30);
	glBindTexture(GL_TEXTURE_2D, text->getTexture());
	glUniform1i(locationTexture, GL_TEXTURE30);
}