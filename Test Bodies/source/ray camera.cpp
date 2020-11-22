#include "ray camera.h"

RayCamera::RayCamera(glm::vec3 position, glm::vec2 rotation, float FOV)
{
	RayCamera::position = position;
	RayCamera::rotation = rotation;
	RayCamera::FOV = FOV;

	//create fullscreen quad

	//vertices
	m_QuadVertices[0] = -1.0f;
	m_QuadVertices[1] = -1.0f;

	m_QuadVertices[2] =  1.0f;
	m_QuadVertices[3] = -1.0f;
	
	m_QuadVertices[4] = -1.0f;
	m_QuadVertices[5] =  1.0f;

	m_QuadVertices[6] =  1.0f;
	m_QuadVertices[7] =  1.0f;

	//indices
	m_QuadIndices[0] = 0;
	m_QuadIndices[1] = 1;
	m_QuadIndices[2] = 2;

	m_QuadIndices[3] = 3;
	m_QuadIndices[4] = 1;
	m_QuadIndices[5] = 2;

	//vertex array
	m_QuadVArray = vertexArray();

	vertexBuffer(m_QuadVertices, 8 * sizeof(float), true);
	indexBuffer(m_QuadIndices, 6 * sizeof(unsigned int), true);

	//positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	//ray shader
	m_RayShader = createShader(loadShader("shaders/cellrayv.shader"), loadShader("shaders/cellrayf.shader"));
}

unsigned int RayCamera::getShader()
{
	return m_RayShader;
}

unsigned int RayCamera::getVArray()
{
	return m_QuadVArray;
}

glm::mat4 RayCamera::getRotationMatrix()
{
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	return rotY * rotX;
}