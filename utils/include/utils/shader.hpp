#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

#include <glad/glad.h>

class Shader
{
    public:
        Shader(const char* const vertexPath, const char* const fragmentPath);

        inline void use() const
        {
            glUseProgram(ID);
        }

        inline void setBool(const std::string& name, const bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<GLint>(value));
        }

        inline void setInt(const std::string& name, const int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        inline void setFloat(const std::string& name, const float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

        inline unsigned int program() const
        {
            return ID;
        }
    private:
        unsigned int ID;
};

void shaderCompileStatus(const unsigned int shader, const char* const which);
void vertexShaderCompileStatus(const unsigned int shader);
void fragmentShaderCompileStatus(const unsigned int shader);
void shaderLinkStatus(const unsigned int shader);

#endif // _SHADER_H_