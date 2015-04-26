#include "ShaderImporter.h"

#include <fstream>
#include <iostream>
#include <string>



ShaderImporter::ShaderImporter()
{
	std::string path;
	path = "Shaders//Vertex//simple_vertex_shader.glsl";
	loadShader(path, Shader::ShaderType::VERTEX);
	path = "Shaders//Fragment//simple_fragment_shader.glsl";
	loadShader(path, Shader::ShaderType::FRAGMENT);

	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->setVertexShader(shaders.find("Shaders//Vertex//simple_vertex_shader.glsl")->second);
	shaderProgram->setFragmentShader(shaders.find("Shaders//Fragment//simple_fragment_shader.glsl")->second);
	shaderProgram->buildProgram();
	shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("defaultShader", shaderProgram));



	path = "Shaders//Vertex//texture_vertex_shader.glsl";
	loadShader(path, Shader::ShaderType::VERTEX);
	path = "Shaders//Fragment//texture_fragment_shader.glsl";
	loadShader(path, Shader::ShaderType::FRAGMENT);

	shaderProgram = new ShaderProgram();
	shaderProgram->setVertexShader(shaders.find("Shaders//Vertex//texture_vertex_shader.glsl")->second);
	shaderProgram->setFragmentShader(shaders.find("Shaders//Fragment//texture_fragment_shader.glsl")->second);
	shaderProgram->buildProgram();
	shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("textureShader", shaderProgram));


	path = "Shaders//Vertex//lighting_vertex_shader.glsl";
	loadShader(path, Shader::ShaderType::VERTEX);
	path = "Shaders//Fragment//lighting_fragment_shader.glsl";
	loadShader(path, Shader::ShaderType::FRAGMENT);

	shaderProgram = new ShaderProgram();
	shaderProgram->setVertexShader(shaders.find("Shaders//Vertex//lighting_vertex_shader.glsl")->second);
	shaderProgram->setFragmentShader(shaders.find("Shaders//Fragment//lighting_fragment_shader.glsl")->second);
	shaderProgram->buildProgram();
	shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("lightingShader", shaderProgram));



	path = "Shaders//Vertex//shadow_vertex.glsl";
	loadShader(path, Shader::ShaderType::VERTEX);
	path = "Shaders//Fragment//shadow_fragment.glsl";
	loadShader(path, Shader::ShaderType::FRAGMENT);

	shaderProgram = new ShaderProgram();
	shaderProgram->setVertexShader(shaders.find("Shaders//Vertex//shadow_vertex.glsl")->second);
	shaderProgram->setFragmentShader(shaders.find("Shaders//Fragment//shadow_fragment.glsl")->second);
	shaderProgram->buildProgram();
	shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("shadowShader", shaderProgram));


	path = "Shaders//Vertex//animation.vert";
	loadShader(path, Shader::ShaderType::VERTEX);
	path = "Shaders//Fragment//animation.frag";
	loadShader(path, Shader::ShaderType::FRAGMENT);

	shaderProgram = new ShaderProgram();
	shaderProgram->setVertexShader(shaders.find("Shaders//Vertex//animation.vert")->second);
	shaderProgram->setFragmentShader(shaders.find("Shaders//Fragment//animation.frag")->second);
	shaderProgram->buildProgram();
	shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("animationShader", shaderProgram));
}


ShaderProgram* ShaderImporter::getShaderProgram(std::string shaderProgramIdentifier)
{
	return shaderPrograms.find(shaderProgramIdentifier)->second;
}

ShaderImporter* ShaderImporter::getInstance()
{
	static ShaderImporter INSTANCE;
	return &INSTANCE;
}

Shader* ShaderImporter::loadShader(std::string path, Shader::ShaderType type)
{
	if (shaders.find(path) != shaders.end()) 
		return shaders.find(path)->second;
	Shader* shader = new Shader(type, path);
	GLuint shaderId;
	
	switch (type){
	case Shader::ShaderType::VERTEX: 
		shaderId = glCreateShader(GL_VERTEX_SHADER);
		break;
	case Shader::ShaderType::FRAGMENT:
		shaderId = glCreateShader(GL_FRAGMENT_SHADER); 
		break;
	case Shader::ShaderType::GEOMETRY:
		shaderId = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	default: std::cerr << "shader type doesn't exist" << std::endl;
	}
	
	std::string shaderCode;
	std::ifstream codeStream(path, std::ios::in);
	if (codeStream.is_open())
	{
		std::string line = "";
		while (getline(codeStream, line))
			shaderCode += "\n" + line;
		codeStream.close();
	}

	std::cerr << "compiling shader: " << path << std::endl;
	char const * sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	GLint result = GL_FALSE;
	int logLength;

	// Check Shader
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> shaderErrorMessage(logLength);
	glGetShaderInfoLog(shaderId, logLength, NULL, shaderErrorMessage.data());
	fprintf(stdout, "%s\n", &shaderErrorMessage[0]);

	shader->setShaderId(shaderId);
	shaders.insert(std::pair<std::string, Shader*>(path, shader));

	return shader;
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
