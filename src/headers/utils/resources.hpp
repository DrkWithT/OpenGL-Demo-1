#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>

namespace resources
{
    class GLShader
    {
    private:
        char* source;
        uint32_t id;
        bool load_ok;

    public:
        GLShader() = default;
        GLShader(uint32_t type, const char* file_path);
        ~GLShader();

        [[nodiscard]] uint32_t getId() const;
        [[nodiscard]] bool getLoadFlag() const;
        [[nodiscard]] bool compile(char info_buffer[], int32_t info_size);
    };

    class GLShaderProgram
    {
    private:
        uint32_t id;
        bool setup_ok;

    public:
        GLShaderProgram() = default;
        GLShaderProgram(const char* vshader_path, const char* fshader_path);

        [[nodiscard]] bool getSetupFlag() const;

        void setUniformBool(const std::string& name, bool value) const;
        void setUniformInt(const std::string& name, int32_t value) const;
        void setUniformFloat(const std::string& name, float value) const;

        void use();
    };

    /// @todo Create ResourceLoader class later!
}

#endif