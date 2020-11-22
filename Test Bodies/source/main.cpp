#include <iostream>

#include <iostream>
#include <chrono>
#include <string.h>

#define GLEW_STATIC
#include <glew.h>
#include <glfw3.h>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "input.h"
#include "vertex array.h"
#include "shader.h"

#include "render.h"

int main()
{
	GLFWwindow* window;

	//initialize the GLFW library
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	//create a windowed mode window and its OpenGL context
	const float windowWidth = 1000;
	const float windowHeight = 700;

	window = glfwCreateWindow(windowWidth, windowHeight, "Cellular Sim", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make the window's context current
	glfwMakeContextCurrent(window);

	//check the GLEW library
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew IS NOT ok!!" << std::endl;
	}

	//output OpenGL version
	std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

	//set blend stuff
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initialize variables
	unsigned int FPS = 20;
	auto loopStart = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> loopDur;

	int winX, winY;
	glfwGetWindowPos(window, &winX, &winY);
	setMousePos(glm::vec2(winX + 10.0f, winY + 10.0f));
	glm::vec2 firstMousePos = getMousePos();

	//initialize camera
	RayCamera cam(glm::vec3(50.0f, 50.0f, 280.0f), glm::vec2(0.0f, 0.0f), glm::radians(45.0f));

	//initialize cell stuff
	CellBlock block(101);

	//initialize renderer
	Renderer r(&cam, windowWidth, windowHeight);

	//swap buffers
	glfwSwapBuffers(window);

	//unbind all

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	while (!glfwWindowShouldClose(window))
	{
		loopStart = std::chrono::high_resolution_clock::now();

		srand(time(NULL));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		r.render(block);
		if (leftMouseDown())
			block.update();

		float distance = 20.0f / (float)FPS;
		glm::vec3 movement1 = distance * glm::vec3(sin(glm::radians(cam.rotation.y)), 0.0f, cos(glm::radians(cam.rotation.y)));
		glm::vec3 movement2 = distance * glm::vec3(sin(glm::radians(cam.rotation.y + 90.0f)), 0.0f, cos(glm::radians(cam.rotation.y + 90.0f)));

		if (keyDown(key::W))
		{
			cam.position -= movement1;
		}
		if (keyDown(key::S))
		{
			cam.position += movement1;
		}
		if (keyDown(key::A))
		{
			cam.position -= movement2;
		}
		if (keyDown(key::D))
		{
			cam.position += movement2;
		}
		if (keyDown(key::LSHIFT))
		{
			cam.position -= glm::vec3(0.0f, distance, 0.0f);
		}
		if (keyDown(key::SPACE))
		{
			cam.position += glm::vec3(0.0f, distance, 0.0f);
		}

		glm::vec2 mouseChange = 3.0f * (firstMousePos - getMousePos()) / (float)FPS;

		cam.rotation.y += mouseChange.x;
		if (cam.rotation.x + mouseChange.y <= 90.0f and cam.rotation.x + mouseChange.y >= -90.0f)
		{
			cam.rotation.x += mouseChange.y;
		}

		setMousePos(firstMousePos);

		if (keyDown(key::ESC))
			break;

		//swap front and back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();

		//adjust for framerate
		//loopDur = std::chrono::high_resolution_clock::now() - loopStart;
		//std::cout << (loopDur.count() / (1.0f / FPS)) * 100 << "% of alloted time" << std::endl;
		loopDur = std::chrono::high_resolution_clock::now() - loopStart;

		while (loopDur.count() < 1.0f / FPS)
		{
			loopDur = std::chrono::high_resolution_clock::now() - loopStart;
		}
	}
}