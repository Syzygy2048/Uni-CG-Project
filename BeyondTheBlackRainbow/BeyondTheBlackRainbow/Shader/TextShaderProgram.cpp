#include "TextShaderProgram.h"


TextShaderProgram::TextShaderProgram(GLuint shaderProgramID) : ShaderProgram(shaderProgramID)
{
	
}


TextShaderProgram::~TextShaderProgram()
{
}

void TextShaderProgram::loadUniformLocations()
{
	locationTexture = glGetUniformLocation(programId, "myTextureSampler");
}

void TextShaderProgram::fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights)
{	
}

void TextShaderProgram::fillUniformLocation(Text* text)
{
	glActiveTexture(GL_TEXTURE30);
	glBindTexture(GL_TEXTURE_2D, text->getTexture()->getTextureID());
	glUniform1i(locationTexture, 30);
}

void TextShaderProgram::fillUniformLocation(Framebuffer* frameBuffer)
{

}
