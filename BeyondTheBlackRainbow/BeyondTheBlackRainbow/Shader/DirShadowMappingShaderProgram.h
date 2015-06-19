#pragma once

#include "..\Shader\ShaderProgram.h"
#include "..\Texture\Texture.h"

class DirShadowMappingShaderProgram : public ShaderProgram
{
public:
	DirShadowMappingShaderProgram(GLuint shaderProgramID);
	~DirShadowMappingShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights);
	virtual void fillUniformLocation(Text* text);
	virtual void fillUniformLocation(Framebuffer* frameBuffer);


private:

	GLuint locationMVP;
	GLuint locationMV;
	GLuint locationV;
	GLuint locationM;
	GLuint locationLightDirection;
	GLuint locationLightPosition;
	GLuint locationDepthBiasMVP;
	GLuint locationTexture;
	GLuint locationShadowMap;

	void useLights(std::vector<LightNode*> lights);
	void bindTextures(MeshNode* node);
};

