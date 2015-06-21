#pragma once

#include "../Shader/ShaderProgram.h"

class LightingShaderProgram : public ShaderProgram
{
public:
	LightingShaderProgram(GLuint shaderProgramID);
	~LightingShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights);
	virtual void fillUniformLocation(Text* text);
	virtual void fillUniformLocation(Framebuffer* frameBuffer);

	GLuint getLocationMVP();
	GLuint getLocationTexture();
	
private:
	GLuint locationMVP;
	GLuint locationV;
	GLuint locationM;
	GLuint locationLightInvDirection_worldspace;
	GLuint locationDepthBiasMVP;
	

};

