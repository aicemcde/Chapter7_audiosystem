#include "Shader.h"
#include <sstream>
#include <fstream>
#include <string>
#include <glew.h>
#include <SDL.h>

Shader::Shader()
	:mShaderProgram(0)
	,mVertexShader(0)
	,mFragShader(0)
{

}

Shader::~Shader()
{

}

bool Shader::CompileShader(const std::string& fileName,
	GLenum  shaderType,
	GLuint& outShader)
{
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		outShader = glCreateShader(shaderType);
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Shader compilation failed : %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found : %s", fileName.c_str());
		return false;
	}
	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL compilation failed : \n%s", buffer);
		return false;
	}
	return true;
}

bool Shader::Load(const std::string& vertName,
	const std::string& fragName)
{
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);
	if (!IsValidProgram())
	{
		return false;
	}
	return true;
}

bool Shader::IsValidProgram()
{
	GLint status;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("Shader compilation failed : \n%s", buffer);
		return false;
	}
	return true;
}

void Shader::SetActive()
{
	glUseProgram(mShaderProgram);
}

void Shader::Unload()
{
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniformMatrix4fv(
		loc,
		1,
		GL_TRUE,
		matrix.GetAsFloatPtr()
	);
}

void Shader::SetVectorUniform(const char* name, const Vector3& uni)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform3fv(
		loc,
		1,
		uni.GetAsFloatPtr()
	);
}

void Shader::SetFloatUniform(const char* name, const float uni)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	glUniform1f(
		loc,
		uni
	);
}