#pragma once
#include <string>

class Shader
{
public:
	// constructor reads and builds the shader
	Shader(const char* vertex_path, const char* fragment_path);

	// use/activate the shader 
	void Use(); 

	unsigned int id() const { return id_; }

	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const; 
	void SetFloat(const std::string& name, float value) const;
	void SetFloat2(const std::string& name, float v0, float v1) const;
	void SetFloat4(const std::string& name, float v1, float v2, float v3, float v4) const;

private: 
	std::string ReadSourceFile(const char* path);
	void CompileShaderProgram(const char* vertex_source, const char* fragement_source, unsigned int& shader_program);

private:
	unsigned int id_;
};