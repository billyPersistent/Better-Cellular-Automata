#version 430 core

layout(rgba32f) uniform image3D u_CellTextureRead;
layout(rgba32f) uniform image3D u_CellTextureWrite;

layout(local_size_x = 101, local_size_y = 1, local_size_z = 1) in;//max size is 1024

int neighbors(ivec3 textureIndex, int range)
{
	int n = int(pow(1 + range * 2, 3) - 1);

	for (int xi = -range; xi <= range; xi++)
	{
		for (int yi = -range; yi <= range; yi++)
		{
			for (int zi = -range; zi <= range; zi++)
			{
				if (!(xi == 0 && yi == 0 && zi == 0))
				{
					n -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(xi, yi, zi)).a < 1.0);
				}
			}
		}
	}

	return n;
}

int neighbors(ivec3 textureIndex)
{
	int n = 26;

	for (int xi = -1; xi <= 1; xi++)
	{
		for (int yi = -1; yi <= 1; yi++)
		{
			for (int zi = -1; zi <= 1; zi++)
			{
				if (!(xi == 0 && yi == 0 && zi == 0))
				{
					n -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(xi, yi, zi)).a < 1.0);
				}
			}
		}
	}

	return n;
}

int sideNeighbors(ivec3 textureIndex)
{
	int sn = 6;

	sn -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(1, 0, 0)).a < 1.0);
	sn -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(-1, 0, 0)).a < 1.0);
	sn -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(0, 1, 0)).a < 1.0);
	sn -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(0, -1, 0)).a < 1.0);
	sn -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(0, 0, 1)).a < 1.0);
	sn -= int(imageLoad(u_CellTextureRead, textureIndex + ivec3(0, 0, -1)).a < 1.0);

	return sn;
}

void main()
{
	ivec3 textureIndex = ivec3(gl_GlobalInvocationID);

	int n1 = neighbors(textureIndex, 1);
	int n2 = neighbors(textureIndex, 2);
	int sn = sideNeighbors(textureIndex);

	if (n1 > 0 && sn % 2 == 1)
	{
		imageStore(u_CellTextureWrite, textureIndex, vec4(vec3(n1 / 26.0, 0.0, n1 / 26.0), 1.0));
	}
	/*else if (n1 > 10)
	{
		imageStore(u_CellTextureWrite, textureIndex, vec4(0.0));
	}*/
}