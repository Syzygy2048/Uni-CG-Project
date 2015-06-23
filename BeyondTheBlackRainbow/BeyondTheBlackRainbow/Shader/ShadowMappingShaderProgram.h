#pragma once
#include "..\Shader\ShaderProgram.h"
#include "..\Texture\Texture.h"
class ShadowMappingShaderProgram : public ShaderProgram
{
public:
	ShadowMappingShaderProgram(GLuint shaderProgramID);
	~ShadowMappingShaderProgram();
	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights);
	virtual void fillUniformLocation(Text* text);
	virtual void fillUniformLocation(Framebuffer* frameBuffer);
private:
	GLuint locationMVP;
	GLuint locationM;
	GLuint locationDiffuseTexture;
	GLuint locationViewPos;
	GLuint locationShadows;

	void useLights(std::vector<LightNode*> lights);
	void bindTextures(MeshNode* node);
};