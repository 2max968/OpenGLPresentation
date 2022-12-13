#define GLEW_STATIC
#include <GL/glew.h>
#include "renderer.h"
#include "shadercompiler.h"

static unsigned int _vbo;
static unsigned int _vao;
static unsigned int _shader;

bool prepareRenderer()
{
    if (!compileShaderFile(&_shader, "shader.vs", "shader.fs", std::cerr))
        return false;

    float vertices[] = {
        //    x       y     z       r  g  b  a      u  v
            -0.5,   -0.5,   0,      1, 0, 0, 1,     0, 0,
             0.5,   -0.5,   0,      0, 1, 0, 1,     1, 0,
             0.5,    0.5,   0,      0, 0, 1, 1,     1, 1,

            -0.5,   -0.5,   0,      1, 0, 0, 1,     0, 0,
             0.5,    0.5,   0,      0, 0, 1, 1,     1, 1,
            -0.5,    0.5,   0,      1, 1, 0, 1,     0, 1
    };

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glUseProgram(_shader);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

    return true;
}

void render()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void translate(unsigned int shader, float x, float y, float z)
{
    float matrix[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        x, y, z
    };

    glUseProgram(_shader);
    auto modelview = glGetUniformLocation(_shader, "modelView");
    glUniformMatrix4x3fv(modelview, 1, false, matrix);
}