#pragma once
#include <string>
#include <iostream>

bool compileShaderSource(unsigned int *shader,
    const std::string& vertex,
    const std::string& fragment,
    std::ostream& errorStream = std::cerr);

bool compileShaderFile(unsigned int *shader,
    const std::string& vertexFilename,
    const std::string& fragmentFilename,
    std::ostream& errorStream = std::cerr);