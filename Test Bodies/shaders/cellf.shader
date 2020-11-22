#version 430 core

layout(location = 0) out vec4 color;

in vec4 f_CellColor;

void main()
{
	vec4 fragColor = f_CellColor;//lod?

	if (fragColor.a == 0.0)
	{
		discard;
	}
	color = fragColor;
}