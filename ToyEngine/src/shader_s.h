#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream> 
#include <sstream>
#include <iostream>

class Shader
{
public:
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader 
	void use(); 

	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const; 
	void setFloat(const std::string& name, float value) const;
	void setFloat2(const std::string& name, float v0, float v1) const;
	void setFloat4(const std::string& name, float v1, float v2, float v3, float v4) const;

private: 
	std::string readSourceFile(const char* path);
	void compileShaderProgram(const char* vertexsource, const char* fragementsource, unsigned int& shaderProgram);

public:
	unsigned int ID;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// read shader from file
	std::string vShaderCode = readSourceFile(vertexPath);
	std::string fShaderCode = readSourceFile(fragmentPath);

	// compile the shaders
	compileShaderProgram(vShaderCode.c_str(), fShaderCode.c_str(), ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat2(const std::string& name, float v0, float v1) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), v0, v1);
}

void Shader::setFloat4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

std::string Shader::readSourceFile(const char* path)
{
	// retrieve source code from filePath
	std::string s_code;
	std::ifstream shaderFile;

	// ensure ifstream objects can throw exceptions
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		shaderFile.open(path);

		// read file's buffer contents into streams 
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		// close file handlers 
		shaderFile.close();

		// convert stream into string 
		s_code = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return s_code;
}


void Shader::compileShaderProgram(const char* vertexsource, const char* fragementsource, unsigned int& shaderProgram)
{
	int success;
	char infoLog[512];

	// compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexsource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\N" << infoLog << std::endl;
	}

	// compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragementsource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create shader program
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADERPROGRAM::COMPILIATION_FAILED\n" << infoLog << std::endl;
	}

	// Clean-up. Delete shaders. 
	// They are linked into our program now and no longer necessary.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


#endif