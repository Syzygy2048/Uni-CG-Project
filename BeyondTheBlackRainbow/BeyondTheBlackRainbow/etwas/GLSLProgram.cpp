#include "GLSLProgram.h"
#include <fstream>
#include <vector>


GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
{
	
}


GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::complieShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		fprintf(stderr, "Error _vertexShaderID");
	}
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		fprintf(stderr, "Error _fragmentShaderID");
	}	
	complieShader(vertexShaderFilePath, _vertexShaderID);
	complieShader(fragmentShaderFilePath, _fragmentShaderID);
}

void GLSLProgram::complieShader(const std::string&filePath, GLuint id) {
	_programID = glCreateProgram();
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		fprintf(stderr, "ShaderFile");
	}
	std::string fileContents = "";
	std::string line;

	while (std::getline(shaderFile, line)) {
		fileContents += line + "\n";
	}

	shaderFile.close();

	const char* contentPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentPtr, nullptr);
	glCompileShader(id);
}

void GLSLProgram::linkShaders() {
	

	//Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(_programID);
		//Don't leak shaders either.
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		//Use the infoLog as you see fit.

		//In this simple program, we'll just leave
		return;
	}

	//Always detach shaders after a successful link.
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::use()
{
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}
void GLSLProgram::unuse()
{
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}
void GLSLProgram::addAttribute(const std::string& attributeName)
{
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}
