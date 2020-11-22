#pragma once

#include "cell.h"

class RayCamera
{
private:
	float m_QuadVertices[8];
	unsigned int m_QuadIndices[6];
	unsigned int m_QuadVArray;

	unsigned int m_RayShader;

public:
	glm::vec3 position;
	glm::vec2 rotation;

	float FOV;
public:
	RayCamera(glm::vec3 position, glm::vec2 rotation, float FOV);

	unsigned int getShader();
	unsigned int getVArray();
	glm::mat4 getRotationMatrix();
};