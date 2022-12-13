#version 310 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUv;

uniform mat4x3 modelView;

out vec4 color;
out vec2 uv;

void main()
{
    gl_Position = vec4(modelView * vec4(aPos.x, aPos.y, aPos.z, 1.0), 1.0);
    color = aColor;
    uv = aUv;
}
