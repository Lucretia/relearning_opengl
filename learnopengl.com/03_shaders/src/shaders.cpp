#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <utils/shader.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Triangle.
const float vertices[] = {
    //  Positions       Colours
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

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
    Shader ourShader("../learnopengl.com/03_shaders/src/shader.vs", "../learnopengl.com/03_shaders/src/shader.fs");

    unsigned int vao;
    unsigned int vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Have to bind the VAO before the VBO, then set up the vertex attributes.
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // Rendering.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        const float timeValue  = glfwGetTime();
        const float greenValue = sin(timeValue) / 2.0 + 0.5f;
        const int   vertexColourLocation = glGetUniformLocation(ourShader.program(), "ourColour");

        glUniform4f(vertexColourLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
        glBindVertexArray(0);

        // Check/call events and then swap the render buffers.
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Render loop.

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

