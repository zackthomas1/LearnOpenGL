#pragma once
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace LearnOpenGL
{
    class Shader
    {
    public:
        // constructor reads and builds the shader
        Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path="");

        // use/activate the shader 
        void Use();

        unsigned int id() const { return id_; }

        // utility uniform functions
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, float v0, float v1) const;
        void SetVec2(const std::string& name, glm::vec2 value) const;
        void SetVec3(const std::string& name, float v1, float v2, float v3) const;
        void SetVec3(const std::string& name, glm::vec3 v1) const;
        void SetVec4(const std::string& name, float v1, float v2, float v3, float v4) const;
        void SetMat4(const std::string& name, glm::mat4 value);

    private:
        std::string ReadSourceFile(const char* path);
        void CompileShaderProgram(const char* vertex_source, const char* fragement_source, unsigned int& shader_program);
        void CompileShaderProgram(const char* vertex_source, const char* fragement_source, const char* geometry_source, unsigned int& shader_program);

    private:
        unsigned int id_;
    };
}