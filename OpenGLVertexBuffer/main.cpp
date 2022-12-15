#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Vertex Buffer", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    prepareRenderer();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        render();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}