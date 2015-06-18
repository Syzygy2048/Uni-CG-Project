#pragma once

#include "../Shader/ShaderProgram.h"

class LightingShaderProgram : public ShaderProgram
{
public:
	LightingShaderProgram(GLuint shaderProgramID);
	~LightingShaderProgram();

	void loadUniformLocations();
	void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights);
	void fillUniformLocation(Text* text);
	void fillUniformLocation(GLuint textureUnit);

	GLuint getLocationMVP();
	GLuint getLocationTexture();
	
private:
	GLuint locationMVP;
	GLuint locationV;
	GLuint locationM;
	GLuint locationLightInvDirection_worldspace;
	GLuint locationDepthBiasMVP;
	

};

