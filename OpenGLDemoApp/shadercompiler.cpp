#include "shadercompiler.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#define INFO_LOG_SIZE   512

bool compileShaderSource(unsigned int *shader,
    const std::string& vertex,
    const std::string& fragment,
    std::ostream& errorStream)
{
    int success;
    char infoLog[INFO_LOG_SIZE];

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* text = vertex.c_str();
    glShaderSource(vertexShader, 1, &text, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        int length;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
        char *buffer = new char[length];
        glGetShaderInfoLog(vertexShader, length, &length, buffer);
        errorStream << "Vertex-shader compilation error:\n" << buffer << std::endl;
        delete[] buffer;
        return false;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    text = fragment.c_str();
    glShaderSource(fragmentShader, 1, &text, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, INFO_LOG_SIZE, nullptr, infoLog);
        errorStream << "Fragment-shader compilation error:\n" << infoLog << std::endl;
        glDeleteShader(vertexShader);
        return false;
    }

    *shader = glCreateProgram();
    glAttachShader(*shader, vertexShader);
    glAttachShader(*shader, fragmentShader);
    glLinkProgram(*shader);
    glGetProgramiv(*shader, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(*shader, INFO_LOG_SIZE, nullptr, infoLog);
        errorStream << "Shader linking error:\n" << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

bool compileShaderFile(unsigned int *shader,
    const std::string& vertexFilename,
    const std::string& fragmentFilename,
    std::ostream& errorStream)
{
    if(!std::filesystem::exists(vertexFilename))
    {
        errorStream << "File '" << vertexFilename << "' does not exist" << std::endl;
        return false;
    }

    if(!std::filesystem::exists(fragmentFilename))
    {
        errorStream << "File '" << fragmentFilename << "' does not exist" << std::endl;
        return false;
    }

    std::ifstream vertexStream, fragmentStream;

    size_t vertexLength = std::filesystem::file_size(vertexFilename);
    vertexStream.open(vertexFilename, std::ios_base::in);

    char* vertex = new char[vertexLength + 1];
    vertexStream.read(vertex, vertexLength);
    vertex[vertexLength] = '\0';
    vertexStream.close();

    size_t fragmentLength = std::filesystem::file_size(fragmentFilename);
    fragmentStream.open(fragmentFilename, std::ios_base::in);

    char* fragment = new char[fragmentLength + 1];
    fragmentStream.read(fragment, fragmentLength);
    fragment[fragmentLength] = '\0';
    fragmentStream.close();

    bool result = compileShaderSource(shader, vertex, fragment, errorStream);
    delete[] vertex;
    delete[] fragment;
    return result;
}