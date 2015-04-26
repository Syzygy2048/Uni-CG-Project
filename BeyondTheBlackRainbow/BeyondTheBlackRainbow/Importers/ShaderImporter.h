#pragma once

#include <GL\glew.h>
#include <vector>
#include <string>
#include <map>

#include "../Shader/Shader.h"
#include "../shader/ShaderProgram.h"

class ShaderImporter
{
public:

	//GLuint loadShaderArray(std::vector<std::string> shaderPaths);
	
	Shader* loadShader(std::string, Shader::ShaderType type);
	ShaderProgram* getShaderProgram(std::string shaderProgramIdentifier);

	static ShaderImporter* getInstance();
	~ShaderImporter();

private:
	ShaderImporter();
	ShaderImporter(ShaderImporter const&);
	void operator=(ShaderImporter const&);
	
	std::map<std::string, Shader*> shaders;
	std::map<std::string, ShaderProgram*> shaderPrograms;

	
};

