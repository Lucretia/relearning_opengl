#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void shaderCompileStatus(const unsigned int shader, const char* const which);
void vertexShaderCompileStatus(const unsigned int shader);
void fragmentShaderCompileStatus(const unsigned int shader);
void shaderLinkStatus(const unsigned int shader);

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0f);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColour;\n"
    "uniform vec4 ourColour;\n"
    "void main()\n"
    "{\n"
    "    FragColour = ourColour;\n"
    "}\0";

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int windowWidth  = 1280;
    const int windowHeight = 1024;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL - Shaders", NULL, NULL);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard_callback);

    // This cannot be any earlier otherwise it fails.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;

        return -1;
    }

    int numAttributes;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);

    std::cout << "Max. number of vertex attributes: " << numAttributes << std::endl;

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
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int vao;
    unsigned int vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Have to bind the VAO before the VBO, then set up the vertex attributes.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // Rendering.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        const float timeValue  = glfwGetTime();
        const float greenValue = sin(timeValue) / 2.0 + 0.5f;
        const int   vertexColourLocation = glGetUniformLocation(shaderProgram, "ourColour");

        glUniform4f(vertexColourLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
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

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    static bool fillModeOn = true;

    switch (key)
    {
        case GLFW_KEY_F:
            if (action == GLFW_PRESS)
                {
                    if (fillModeOn)
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    }
                    else
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }

                    fillModeOn = !fillModeOn;
                }
            break;

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);

            break;
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
