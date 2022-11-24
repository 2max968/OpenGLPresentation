#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "RenderFunctions.h"

int main()
{
    int error = glfwInit();
    if (error != GL_TRUE)
    {
        std::cerr << "Error loading GLFW: " << glewGetErrorString(error) << std::endl;
        return -1;
    }
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    error = glewInit();
    if (error != GLEW_OK)
    {
        std::cerr << "Error loading GLEW: " << glewGetErrorString(error) << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    auto startTime = std::chrono::steady_clock::now();
    int texture = LoadTexture();

    while (!glfwWindowShouldClose(window))
    {
        float time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() / 1000.0;

        glfwPollEvents();
        glfwMakeContextCurrent(window);
        int windowW, windowH;
        glfwGetWindowSize(window, &windowW, &windowH);
        glViewport(0, 0, windowW, windowH);

        RedTriangle();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}