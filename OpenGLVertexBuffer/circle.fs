#version 310 es
precision mediump float;

in vec4 color;
in vec2 uv;

out vec4 FragColor;

void main()
{
	if(length(uv - 0.5) > 0.5)
	{
		discard;
		return;
	}
    FragColor = color;
}
