#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>


ShaderProgram::ShaderProgram() : mHandle(0)
{

}

ShaderProgram::~ShaderProgram()
{
	if (mHandle != 0)
	{
		glDeleteProgram(mHandle);
	}
}

bool ShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	// create and compile vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);

	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	// create shader program
	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	checkCompileErrors(mHandle, PROGRAM);

	//  delete shaders - we don't need them anymore
	glDeleteShader(vs);
	glDeleteShader(fs);

	mUniformLocations.clear();

	return true;
}

void ShaderProgram::use()
{
	if (mHandle != 0)
	{
		glUseProgram(mHandle);
	}
	else
	{
		std::cerr << "ERROR: Shader program not loaded" << std::endl;
	}
}

string ShaderProgram::fileToString(const string& filename)
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);
		if (!file.fail())
		{
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR: Could not read shader file: " << filename << std::endl; {}
	}

	return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	if (type == PROGRAM)
	{
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
			std::cerr << "ERROR: Program linking failed\n" << errorLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "ERROR: Shader compilation failed\n" << errorLog << std::endl;
		}
	}

}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);

	if (it == mUniformLocations.end())
	{
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);

	}

	return mUniformLocations[name];
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}