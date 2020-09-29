#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void shaderCompileStatus(const unsigned int shader, const char* const which);
void vertexShaderCompileStatus(const unsigned int shader);
void fragmentShaderCompileStatus(const unsigned int shader);
void shaderLinkStatus(const unsigned int shader);

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColour;\n"
    "void main()\n"
    "{\n"
    "    FragColour = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int windowWidth  = 1280;
    const int windowHeight = 1024;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL - Hello Window", NULL, NULL);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // This cannot be any earlier otherwise it fails.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;

        return -1;
    }

    // Shaders.
    unsigned int vertexShader;
    unsigned int fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    vertexShaderCompileStatus(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    fragmentShaderCompileStatus(fragmentShader);

    unsigned int shaderProgram;

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    shaderLinkStatus(shaderProgram);

    // Triangle.
    const float vertices[] = {
         0.5f,  0.5f, 0.0f, // Top right.
         0.5f, -0.5f, 0.0f, // Bottom right.
        -0.5f, -0.5f, 0.0f, // Bottom left.
        -0.5f,  0.5f, 0.0f  // Top left.
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Have to bind the VAO before the VBO, then set up the vertex attributes.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Rendering.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        // Check/call events and then swap the render buffers.
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Render loop.

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
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

    glGetShaderiv(shader, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, logSize, nullptr ,log);

        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << std::endl;
    }
}
