#pragma once

#include "camera.h"
#include "ray camera.h"

#include "cell.h"

class Renderer
{
private:
	RayCamera* m_Camera;

	float m_WinWidth;
	float m_WinHeight;
public:
	Renderer(RayCamera* cam, float winWidth, float winHeight);

	void render(CellBlock& block);
};