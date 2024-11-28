#include "pch.h"
#include "shader_s.h"

#include <glad/glad.h>


Shader::Shader(const char* vertex_path, const char* fragment_path)
{
	// read shader from file
	std::string vertex_shader_code = ReadSourceFile(vertex_path);
	std::string fragment_shader_code = ReadSourceFile(fragment_path);

	// compile the shaders
	CompileShaderProgram(vertex_shader_code.c_str(), fragment_shader_code.c_str(), id_);
}

void Shader::Use()
{
	glUseProgram(id_);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetFloat2(const std::string& name, float v0, float v1) const
{
	glUniform2f(glGetUniformLocation(id_, name.c_str()), v0, v1);
}

void Shader::SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const
{
	glUniform4f(glGetUniformLocation(id_, name.c_str()), v1, v2, v3, v4);
}

std::string Shader::ReadSourceFile(const char* path)
{
	// retrieve source code from filePath
	std::string source_code;
	std::ifstream shader_file;

	// ensure ifstream objects can throw exceptions
	shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		shader_file.open(path);

		// read file's buffer contents into streams 
		std::stringstream shader_stream;
		shader_stream << shader_file.rdbuf();

		// close file handlers 
		shader_file.close();

		// convert stream into string 
		source_code = shader_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return source_code;
}


void Shader::CompileShaderProgram(const char* vertex_source, const char* fragement_source, unsigned int& shader_program)
{
	int success;
	char info_log[512];

	// compile vertex shader
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\N" << info_log << std::endl;
	}

	// compile fragment shader
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragement_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// Create shader program
	shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cout << "ERROR::SHADERPROGRAM::COMPILIATION_FAILED\n" << info_log << std::endl;
	}

	// Clean-up. Delete shaders. 
	// They are linked into our program now and no longer necessary.
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}
