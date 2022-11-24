#include <GL/glew.h>
#include <GL/gl.h>
#include "RenderCode.h"
#include "shadercompiler.h"

static unsigned int _vbo;
static unsigned int _vao;
static unsigned int _shader;

void prepareShader()
{
    compileShaderFile(&_shader, "shader.vs", "shader.fs", std::cerr);
}

void createVBO()
{
    float vertices[] = {
    //   x     y    z         r  g  b  a
         0.0, -0.5, 0.0,      1, 0, 0, //1,
         0.5,  0.5, 0.0,      0, 1, 0, //1,
        -0.5,  0.5, 0.0,      0, 0, 1//, 1
    };

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)12);
}

void Render()
{
    glUseProgram(_shader);
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}