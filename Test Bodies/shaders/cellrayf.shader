#version 430 core

layout(location = 0) out vec4 color;

in vec2 f_Position;

layout(rgba32f) uniform image3D u_CellTexture;
uniform int u_BlockSize;

uniform vec3 u_CameraPosition;
uniform mat4 u_CenterRayDirection;

uniform float u_FOV;
uniform vec2 u_WindowSize;

float max(vec3 v)
{
	return max(v.x, max(v.y, v.z));
}

float min(vec3 v)
{
	return min(v.x, min(v.y, v.z));
}

float oneZero(float num)
{
	if (num > 0.0)
		return 1.0;
	else
		return 0.0;
}

float reverseOneZero(float num)
{
	if (num > 0.0)
		return 0.0;
	else
		return 1.0;
}

float oneNegative(float num)
{
	if (num > 0.0)
		return 1.0;
	else
		return -1.0;
}

void main()
{
	//create a vector in the ray frustum
	vec3 rayVector = mat3(u_CenterRayDirection) * normalize(vec3(f_Position.x, f_Position.y * (u_WindowSize.y / u_WindowSize.x), -1.0 / tan(u_FOV / 2.0)));

	//cellIndexCandidates stores all components of rayVector normalized to a 1 either -1 depending on sign
	//this shows which faces of an axis aligned cube the ray could exit through, and which it couldn't
	//this is used to increment cellIndex depending on where the ray exited a cell
	ivec3 cellIndexCandidates = ivec3(oneNegative(rayVector.x), oneNegative(rayVector.y), oneNegative(rayVector.z));//always positive?

	//planeOffsetCandidates stores the same thing as cellIndexCandidates, but goes to either 0 or 1
	//this is used to find the position of a cubes face planes given cellIndex
	vec3 planeOffsetCandidates = vec3(oneZero(rayVector.x), oneZero(rayVector.y), oneZero(rayVector.z));
	
	//find the entrance and exit distances of the ray through the cell block
	vec3 rayEntranceDistances;
	vec3 rayExitDistances;

	for (int i = 0; i < 3; i++)
	{
		rayExitDistances[i] = (u_BlockSize * planeOffsetCandidates[i] - u_CameraPosition[i]) / rayVector[i];
		rayEntranceDistances[i] = (u_BlockSize * reverseOneZero(planeOffsetCandidates[i]) - u_CameraPosition[i]) / rayVector[i];
	}

	float lowIntersectionDistance = max(rayEntranceDistances);
	float highIntersectionDistance = min(rayExitDistances);
	
	//stores the distance to the face of the current cell
	float cellEntranceDistance = max(lowIntersectionDistance, 0.0);

	//stores the starting position of the ray
	vec3 blockIntersection = u_CameraPosition + rayVector * cellEntranceDistance;

	//stores the texture index of the current cell
	ivec3 cellIndex = ivec3(floor(blockIntersection + rayVector / 100.0));

	//iterate through all the cells on the ray and end when one is opaque or cell index is outside of the cell block
	while (
		cellIndex.x >= 0 && cellIndex.x < u_BlockSize &&
		cellIndex.y >= 0 && cellIndex.y < u_BlockSize &&
		cellIndex.z >= 0 && cellIndex.z < u_BlockSize &&
		imageLoad(u_CellTexture, cellIndex).a < 1.0)
	{
		float minimumGreaterDistance = cellEntranceDistance + 10.0;
		int exitFaceIndex;

		//loop through all of the cells faces that the ray might exit through and figure out which one it does
		for (int i = 0; i < 3; i++)
		{
			float rayDistance = (cellIndex[i] + planeOffsetCandidates[i] - u_CameraPosition[i]) / rayVector[i];

			if (rayDistance > cellEntranceDistance && rayDistance < minimumGreaterDistance)
			{
				minimumGreaterDistance = rayDistance;
				exitFaceIndex = i;
			}
		}

		//move to the next cell and update cellEntranceDistance
		cellEntranceDistance = minimumGreaterDistance;
		cellIndex[exitFaceIndex] += cellIndexCandidates[exitFaceIndex];
	}

	//add those black frames around the cells
	vec3 rayPosition = u_CameraPosition + rayVector * cellEntranceDistance;
	int onWallCount = 0;
	for (int i = 0; i < 3 && imageLoad(u_CellTexture, cellIndex).a >= 1.0; i++)
	{
		if (distance(rayPosition[i], round(rayPosition[i])) < 0.06)
			onWallCount++;
	}

	//set color
	if (onWallCount < 2)
	{
		color = imageLoad(u_CellTexture, cellIndex);
	}
	else
	{
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}
}