#include <iostream>
#include <chrono>
#include "GLIncludes.h"
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
    glfwSwapInterval(1);
    error = glewInit();
    if (error != GLEW_OK)
    {
        std::cerr << "Error loading GLEW: " << glewGetErrorString(error) << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    auto startTime = std::chrono::steady_clock::now();
    int texture = LoadTexture();
    float lastFrameTime = 0;

    char titleBuffer[100];

    while (!glfwWindowShouldClose(window))
    {
        float time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() / 1000.0;
        float dt = time - lastFrameTime;
        float fps = 1 / dt;
        snprintf(titleBuffer, 100, "FPS: %i", (int)fps);
        glfwSetWindowTitle(window, titleBuffer);
        lastFrameTime = time;

        glfwPollEvents();
        glfwMakeContextCurrent(window);
        int windowW, windowH;
        glfwGetWindowSize(window, &windowW, &windowH);
        glViewport(0, 0, windowW, windowH);

        //RedTriangle();
        ColoredTriangle();
        //TexturedQuad(texture);
        //RotatedTriangle(time);
        Cube(time, windowW, windowH);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
}