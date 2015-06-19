#pragma once

#include "..\Shader\ShaderProgram.h"

class DepthShaderProgram : public ShaderProgram
{
public:
	DepthShaderProgram(GLuint shaderProgramID);
	~DepthShaderProgram();

	virtual void loadUniformLocations();
	virtual void fillUniformLocation(MeshNode* node, std::vector<LightNode*> lights);
	virtual void fillUniformLocation(Text* text);
	virtual void fillUniformLocation(Framebuffer* frameBuffer);

private:
	GLuint locationDepthMVP;
	GLuint locationMV;
	GLuint locationFarPlane;
	GLuint locationLightPos;
	GLuint locationModel;

	void useLights(std::vector<LightNode*> lights);
	void useGeometry(std::vector<glm::mat4> depthTransforms);
};

