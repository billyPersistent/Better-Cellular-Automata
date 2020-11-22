#version 430 core

layout(rgba32f) uniform image3D u_CellTextureRead;
layout(rgba32f) uniform image3D u_CellTextureWrite;

layout(local_size_x = 101, local_size_y = 1, local_size_z = 1) in;

void main()
{
	ivec3 textureIndex = ivec3(gl_GlobalInvocationID);

	imageStore(u_CellTextureRead, textureIndex, imageLoad(u_CellTextureWrite, textureIndex));
}