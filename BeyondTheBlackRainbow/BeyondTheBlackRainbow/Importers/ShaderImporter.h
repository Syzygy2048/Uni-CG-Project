#pragma once

#include <GL\glew.h>
#include <vector>
#include <string>
#include <map>

#include "../shader/ShaderProgram.h"
#include "MeshLoadInfo.h"

class ShaderImporter
{
public:

	//GLuint loadShaderArray(std::vector<std::string> shaderPaths);
	
	GLuint loadShader(std::string shaderPath);
	ShaderProgram* loadShaderProgram(const MeshLoadInfo::ShaderLoadInfo* shader);
	static ShaderImporter* getInstance();
	~ShaderImporter();

private:
	ShaderImporter();
	ShaderImporter(ShaderImporter const&);
	void operator=(ShaderImporter const&);
	
	std::map<const MeshLoadInfo::ShaderLoadInfo*, ShaderProgram*> shaderPrograms;

	
};

