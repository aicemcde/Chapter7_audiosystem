#pragma once
#include <string>
#include <glew.h>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();

	bool Load(const std::string& vertName,
		const std::string& fragName);
	void Unload();

	void SetActive();
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetVectorUniform(const char* name, const Vector3& vec);
	void SetFloatUniform(const char* name, const float uni);
private:
	bool CompileShader(const std::string& fileName,
		GLenum shaderType, GLuint& outShader);
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();

	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};