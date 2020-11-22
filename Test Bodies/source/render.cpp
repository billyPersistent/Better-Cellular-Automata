#include "render.h"

Renderer::Renderer(RayCamera* cam, float winWidth, float winHeight)
{
	m_Camera = cam;

	m_WinWidth = winWidth;
	m_WinHeight = winHeight;
}

void Renderer::render(CellBlock& block)
{
	//bind
	glUseProgram(m_Camera->getShader());
	glBindVertexArray(m_Camera->getVArray());
	glBindImageTexture(0, block.getCellTexture(), 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA32F);

	//set cell sampler
	int imageLocation = glGetUniformLocation(m_Camera->getShader(), "u_CellTexture");
	glUniform1i(imageLocation, 0);//slot

	//set block size
	int blockSizeLocation = glGetUniformLocation(m_Camera->getShader(), "u_BlockSize");
	glUniform1i(blockSizeLocation, block.getSize());//slot

	//set camera position
	int cameraPosLocation = glGetUniformLocation(m_Camera->getShader(), "u_CameraPosition");
	glUniform3f(cameraPosLocation, m_Camera->position.x, m_Camera->position.y, m_Camera->position.z);

	//set center ray direction
	glm::mat4 centerRay = m_Camera->getRotationMatrix();
	int centerRayLocation = glGetUniformLocation(m_Camera->getShader(), "u_CenterRayDirection");
	glUniformMatrix4fv(centerRayLocation, 1, GL_FALSE, (float*)&centerRay);

	//set FOV
	int FOVLocation = glGetUniformLocation(m_Camera->getShader(), "u_FOV");
	glUniform1f(FOVLocation, m_Camera->FOV);

	//set window size
	int winSizeLocation = glGetUniformLocation(m_Camera->getShader(), "u_WindowSize");
	glUniform2f(winSizeLocation, m_WinWidth, m_WinHeight);

	//draw
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}