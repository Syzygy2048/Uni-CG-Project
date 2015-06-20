#include "ShaderImporter.h"

#include <fstream>
#include <iostream>
#include <string>

#include "..\Shader\LightingShaderProgram.h"
#include "..\Shader\TextureShaderProgram.h"
#include "..\Shader\SimpleLightingShaderProgram.h"
#include "..\Shader\TextShaderProgram.h"
#include "..\Shader\BloomShaderProgram.h"
#include "..\Shader\RenderSurfaceShaderProgram.h"
#include "..\Shader\HighPassShaderProgram.h"

ShaderImporter::ShaderImporter()
{
		
}

ShaderImporter* ShaderImporter::getInstance()
{
	static ShaderImporter INSTANCE;
	return &INSTANCE;
}

GLuint ShaderImporter::loadShader(std::string shaderPath)
{
	
	GLuint shaderID;
		if (shaderPath.find("Vertex") != std::string::npos)
			shaderID = glCreateShader(GL_VERTEX_SHADER);
		else if (shaderPath.find("Fragment") != std::string::npos)
			shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		else if (shaderPath.find("Geometry") != std::string::npos)
			shaderID = glCreateShader(GL_GEOMETRY_SHADER);
	
	std::string shaderCode;
	std::ifstream codeStream(shaderPath, std::ios::in);
	if (codeStream.is_open())
	{
		std::string line = "";
		while (getline(codeStream, line))
			shaderCode += "\n" + line;
		codeStream.close();
	}

	std::cerr << "compiling shader: " << shaderPath << std::endl;
	char const * sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	GLint result = GL_FALSE;
	int logLength;

	// Check Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0){
		std::vector<char> shaderErrorMessage(logLength);
		glGetShaderInfoLog(shaderID, logLength, NULL, shaderErrorMessage.data());
		fprintf(stdout, "%s\n", &shaderErrorMessage[0]);	
	}
	return shaderID;
}

ShaderProgram* ShaderImporter::loadShaderProgram(const MeshLoadInfo::ShaderLoadInfo* shader)
{
	if (shaderPrograms.find(shader) != shaderPrograms.end()) {
		return shaderPrograms.find(shader)->second;
	}
	GLuint vertexShaderID = loadShader(shader->vertexShaderPath);
	GLuint fragmentShaderID = loadShader(shader->fragmentShaderPath);
	GLuint shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);
	
	GLint isLinked = 0;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> shaderErrorMessage(maxLength);
		glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &shaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &shaderErrorMessage[0]);
		//std::cerr << infoLog << std::endl;
	}	

	ShaderProgram* result = nullptr;
	if (shader == MeshLoadInfo::LIGHTING_SHADER)
	{
		result = new LightingShaderProgram(shaderProgramID);
	}
	else if (shader == MeshLoadInfo::TEXTURE_SHADER)
	{
		result = new TextureShaderProgram(shaderProgramID);
	}
	else if (shader == MeshLoadInfo::SIMPLE_LIGHTING_SHADER)
	{
		result = new SimpleLightingShaderProgram(shaderProgramID);
	}
	else if (shader == MeshLoadInfo::TEXT_SHADER)
	{
		result = new TextShaderProgram(shaderProgramID);
	}
	else if (shader == MeshLoadInfo::BLOOM_SHADER)
	{
		result = new BloomShaderProgram(shaderProgramID);
	}
	else if (shader == MeshLoadInfo::RENDER_SURFACE){
		result = new RenderSurfaceShaderProgram(shaderProgramID);
	}
	else if (shader == MeshLoadInfo::HIGH_PASS){
		result = new HighPassShaderProgram(shaderProgramID);
	}
	shaderPrograms.insert(std::pair<const MeshLoadInfo::ShaderLoadInfo*, ShaderProgram*>(shader, result));
	return result;
}

/*GLuint ShaderImporter::loadShaderArray(std::vector<std::string> shaderPaths)
{
	GLuint programID = glCreateProgram();

	//for error handling
	GLint result = GL_FALSE;
	int logLength;

	for (const std::string shaderPath : shaderPaths){
		GLuint shaderID;
		if (shaderPath.find("Vertex") != std::string::npos)
			shaderID = glCreateShader(GL_VERTEX_SHADER);
		else if (shaderPath.find("Fragment") != std::string::npos)
			shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		else if (shaderPath.find("Geometry") != std::string::npos)
			shaderID = glCreateShader(GL_GEOMETRY_SHADER);
		//else if (shaderPath.find("TesselationControl") != std::string::npos)
			//shaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
		//else if (shaderPath.find("TesselationEval") != std::string::npos)
			//shaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
		
		std::string shaderCode;
		std::ifstream codeStream(shaderPath, std::ios::in);
		if (codeStream.is_open())
		{
			std::string line = "";
			while (getline(codeStream, line))
				shaderCode += "\n" + line;
			codeStream.close();
		}

		std::cerr << "compiling shader: " << shaderPath << std::endl;
		char const * sourcePointer = shaderCode.c_str();
		glShaderSource(shaderID, 1, &sourcePointer, NULL);
		glCompileShader(shaderID);

		// Check Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> shaderErrorMessage(logLength);	
		//glGetShaderInfoLog(shaderID, logLength, NULL, shaderErrorMessage.data());
		//fprintf(stdout, "%s\n", &shaderErrorMessage[0]);

		glAttachShader(programID, shaderID);
	}
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programErrorMessage(logLength);
	//glGetProgramInfoLog(programID, logLength, NULL, &programErrorMessage[0]);
	//fprintf(stdout, "%s\n", &programErrorMessage[0]);
	return programID;
} */

ShaderImporter::~ShaderImporter()
{
}
