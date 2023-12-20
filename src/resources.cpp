/**
 * @file resources.cpp
 * @author Derek Tan
 * @brief Implements resource utils.
 * @date 2023-12-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "glad/glad.h"

#include <iostream>

#include "utils/resources.hpp"
#include "utils/files.hpp"

/* Constants */

constexpr const int32_t log_buffer_size = 512;

/* GLShader impl. */

resources::GLShader::GLShader(uint32_t type, const char* file_path)
{
    id = glCreateShader(type);
    
    files::FileReader reader {};
    char* source_buffer = nullptr;

    if (!reader.useFile(file_path))
    {
        source = source_buffer;
        load_ok = false;
        return;
    }

    if (!reader.loadFileTo(file_path, source_buffer))
    {
        load_ok = false;
        return;
    }

    source = source_buffer;
    load_ok = true;
}

resources::GLShader::~GLShader()
{
    if (source != nullptr)
    {
        delete[] source;
        source = nullptr;
    }

    glDeleteShader(id);
}

[[nodiscard]] uint32_t resources::GLShader::getId() const
{
    return id;
}

[[nodiscard]] bool resources::GLShader::getLoadFlag() const
{
    return load_ok;
}

[[nodiscard]] bool resources::GLShader::compile(char info_buffer[], int32_t info_size)
{
    bool has_source = getLoadFlag();
    int32_t success_flag = 0;

    if (!has_source)
        return false;

    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success_flag);

    if (!success_flag)
    {
        glGetShaderInfoLog(id, info_size, NULL, info_buffer);
    }

    return success_flag != 0;
}

/* GLShaderProgram impl. */

resources::GLShaderProgram::GLShaderProgram(const char* vshader_path, const char* fshader_path)
{
    id = glCreateProgram();

    char log_buffer[log_buffer_size] = "Read of GLSL file failed.";
    GLShader vert_shader {GL_VERTEX_SHADER, vshader_path};
    GLShader frag_shader {GL_FRAGMENT_SHADER, fshader_path};
    
    if (!vert_shader.compile(log_buffer, log_buffer_size))
    {
        setup_ok = false;
        std::cerr << "Error [Shader Setup]:\n" << log_buffer << '\n';
    }
    
    if (!frag_shader.compile(log_buffer, log_buffer_size))
    {
        setup_ok = false;
        std::cerr << "Error [Shader Setup]:\n" << log_buffer << '\n';
    }

    uint32_t vert_shader_id = vert_shader.getId();
    uint32_t frag_shader_id = frag_shader.getId();
    int32_t link_status = 0;

    glAttachShader(id, vert_shader_id);
    glAttachShader(id, frag_shader_id);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &link_status);

    if (!link_status)
    {
        glGetProgramInfoLog(id, log_buffer_size, NULL, log_buffer);
        std::cerr << "Error [Program Linking]:\n" << log_buffer << '\n';
    }
}

[[nodiscard]] bool resources::GLShaderProgram::getSetupFlag() const
{
    return setup_ok;
}

void resources::GLShaderProgram::setUniformBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int32_t)value);
}

void resources::GLShaderProgram::setUniformInt(const std::string& name, int32_t value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void resources::GLShaderProgram::setUniformFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), (float)value);
}

void resources::GLShaderProgram::use()
{
    glUseProgram(id);
}
