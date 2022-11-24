#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderCode.h"

int main()
{
    // Initializing GLFW
    std::cout << "Init GLFW" << std::endl;
    if (glfwInit() != GLFW_TRUE)
    {
        std::cerr << "GLFW Init failed" << std::endl;
        return 1;
    }

    // Creating OpenGL Window
    std::cout << "Creating OpenGL Window" << std::endl;
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Creating OpenGL window failed" << std::endl;
        return 1;
    }

    // Render Context has to be current, before initializing glew
    glfwMakeContextCurrent(window);

    // Initializing GLEW
    std::cout << "Init Glew" << std::endl;
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        
        std::cerr << "Glew Init failed: " << glewGetErrorString(error) << std::endl;
        return 1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    createVBO();
    prepareShader();

    // Run render-loop until user presses 'X' on window
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwMakeContextCurrent(window);

        Render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}