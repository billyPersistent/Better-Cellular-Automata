#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 textureIndex;

out vec4 f_CellColor;

uniform mat4 u_Rotation;
uniform mat4 u_Camera;

uniform sampler3D u_CellTexture;

void main()
{
	//fetch color here
	gl_Position = u_Camera * u_Rotation * position;

	f_CellColor = texelFetch(u_CellTexture, ivec3(textureIndex), 0);//run on less vertices?
}