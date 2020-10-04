#include <fstream>
#include <sstream>
#include <iostream>

#include <utils/shader.hpp>

Shader::Shader(const char* const vertexPath, const char* const fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    // std::cout << "VS: " << vertexPath << std::endl;
    // std::cout << "FS: " << fragmentPath << std::endl;

    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexStream;
        std::stringstream fragmentStream;

        vertexStream   << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode   = vertexStream.str();
        fragmentCode = fragmentStream.str();
    }
    catch(const std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << '\n';
    }

    const char* const vertexShaderCode   = vertexCode.c_str();
    const char* const fragmentShaderCode = fragmentCode.c_str();

    // std::cout << "Vertex Shader: '" << vertexShaderCode << "'" << std::endl;
    // std::cout << "Fragment Shader: '" << fragmentShaderCode << "'" << std::endl;

    unsigned int vertex;
    unsigned int fragment;

    vertex   = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertex);

    vertexShaderCompileStatus(vertex);

    glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragment);

    fragmentShaderCompileStatus(fragment);

    ID = glCreateProgram();

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    shaderLinkStatus(ID);

    // No longer require these shaders as they are linked into the program.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void shaderCompileStatus(const unsigned int shader, const char* const which)
{
    int success;
    const int logSize = 512;
    char log[logSize];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, logSize, nullptr, log);

        std::cout << "ERROR::SHADER::" << which << "::COMPILATION_FAILED\n" << log << std::endl;
    }
}

void vertexShaderCompileStatus(const unsigned int shader)
{
    shaderCompileStatus(shader, "VERTEX");
}

void fragmentShaderCompileStatus(const unsigned int shader)
{
    shaderCompileStatus(shader, "FRAGMENT");
}

void shaderLinkStatus(const unsigned int shader)
{
    int success;
    const int logSize = 512;
    char log[logSize];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, logSize, nullptr ,log);

        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
    }
}
