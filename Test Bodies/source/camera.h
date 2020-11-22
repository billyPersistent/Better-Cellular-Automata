#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

class Camera
{
private:
	glm::vec3 position;
	glm::vec2 rotation;

	glm::mat4 projection;
	glm::mat4 MVP;

	glm::mat4 getViewM();
	glm::mat4 getRotationM();
public:
	Camera(glm::vec3 pos, glm::vec2 rot, glm::mat4 proj);
	Camera();

	void rotate(glm::vec2 rot);
	void move(glm::vec3 dist);
	void setPos(glm::vec3 pos);

	glm::vec3 getPos();
	glm::vec2 getRotation();
	float* getMatrix();
};