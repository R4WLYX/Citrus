#ifndef SHADER_HPP
#define SHADER_HPP

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "Keywords.hpp"
#include "glm/glm.hpp"

struct ShaderSource {
    std::string 
        Vertex,
        Fragment;
};

class Shader {
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& fileName)
    : m_FilePath(fileName), m_RendererID(0) {
        ShaderSource source = parse_shader(fileName);
        m_RendererID = create_shader(source.Vertex, source.Fragment);
    }
    ~Shader() {
        GLCall(glDeleteProgram(m_RendererID));
    }

    void bind() const {
        GLCall(glUseProgram(m_RendererID));
    }
    void unbind() const {
        GLCall(glUseProgram(0));
    }

#pragma region SetUniforms
    void set_uniform_1i(const std::string& name, int value) {
        GLCall(glUniform1i(get_uniform_location(name), value));
    }
    void set_uniform_2i(const std::string& name, int v0, int v1) {
        GLCall(glUniform2i(get_uniform_location(name), v0, v1));
    }
    void set_uniform_3i(const std::string& name, int v0, int v1, int v2) {
        GLCall(glUniform3i(get_uniform_location(name), v0, v1, v2));
    }
    void set_uniform_4i(const std::string& name, int v0, int v1, int v2, int v3) {
        GLCall(glUniform4i(get_uniform_location(name), v0, v1, v2, v3));
    }
    void set_uniform_2i(const std::string& name, const glm::ivec2 vector) {
        GLCall(glUniform2iv(get_uniform_location(name), 1, &vector[0]));
    }
    void set_uniform_3i(const std::string& name, const glm::ivec3 vector) {
        GLCall(glUniform3iv(get_uniform_location(name), 1, &vector[0]));
    }
    void set_uniform_4i(const std::string& name, const glm::ivec4 vector) {
        GLCall(glUniform4iv(get_uniform_location(name), 1, &vector[0]));
    }

    void set_uniform_1f(const std::string& name, float value) {
        GLCall(glUniform1f(get_uniform_location(name), value));
    }
    void set_uniform_2f(const std::string& name, float v0, float v1) {
        GLCall(glUniform2f(get_uniform_location(name), v0, v1));
    }
    void set_uniform_3f(const std::string& name, float v0, float v1, float v2) {
        GLCall(glUniform3f(get_uniform_location(name), v0, v1, v2));
    }
    void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3) {
        GLCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
    }
    void set_uniform_2f(const std::string& name, const glm::vec2 vector) {
        GLCall(glUniform2fv(get_uniform_location(name), 1, &vector[0]));
    }
    void set_uniform_3f(const std::string& name, const glm::vec3 vector) {
        GLCall(glUniform3fv(get_uniform_location(name), 1, &vector[0]));
    }
    void set_uniform_4f(const std::string& name, const glm::vec4 vector) {
        GLCall(glUniform4fv(get_uniform_location(name), 1, &vector[0]));
    }

    void set_uniform_mat_2f(const std::string& name, const glm::mat2& matrix) {
        GLCall(glUniformMatrix2fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]));
    }
    void set_uniform_mat_3f(const std::string& name, const glm::mat3& matrix) {
        GLCall(glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]));
    }
    void set_uniform_mat_4f(const std::string& name, const glm::mat4& matrix) {
        GLCall(glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]));
    }
#pragma endregion SetUniforms
private:
    ShaderSource parse_shader(const std::string& path) {
        std::ifstream fstream(path);

        enum class ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];

        ShaderType type = ShaderType::NONE;

        while (std::getline(fstream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            } else
                ss[(int)type]<<line<<'\n';

            std::cout<<line<<'\n';
        }
        
        return {ss[0].str(), ss[1].str()};
    }

    unsigned int compile_shader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length*sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cerr<<"Failed To Compile "<<(type == GL_VERTEX_SHADER? "Vertex" : "Fragment")<<" Shader."<<'\n'<<message<<'\n';
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader) {
        GLCall(unsigned int program = glCreateProgram());
        unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    int get_uniform_location(const std::string& name) {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

        if (location == -1)
            std::cout<<"Warning: uniform '"<<name<<"' doesn't exist!\n";
        
        m_UniformLocationCache[name] = location;
        return location;
    }
};

#endif