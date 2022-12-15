#version 310 es

in vec3 aPos;
in vec4 aColor;
in vec2 aUv;

out vec4 color;
out vec2 uv;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    color = aColor;
    uv = aUv;
}
