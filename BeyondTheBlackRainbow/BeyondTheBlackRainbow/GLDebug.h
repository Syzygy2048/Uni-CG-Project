#pragma once

#include <iostream>
#include <sstream>
#include <GL\glew.h>
#include <GL\wglew.h>

class GLDebug
{
public:
	static void registerDebugCallbacks();
	
private: 
	static std::string formatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
	static void APIENTRY GLDebug::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	static void APIENTRY debugCallbackAMD(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
};