#version 430 core

layout(location = 0) in vec4 position;

out vec2 f_Position;

void main()
{
	f_Position = position.xy;

	gl_Position = position;
}