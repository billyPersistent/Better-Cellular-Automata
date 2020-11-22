#pragma once

#include <vector>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "vertex array.h"
#include "shader.h"
#include "texture.h"

struct CellVertex
{
	glm::vec3 position;
	glm::vec3 index;
};

class CellBlock
{
private:
	int m_BlockSize;

	unsigned int m_CellTextureRead;
	unsigned int m_CellTextureWrite;

	unsigned int m_CopyShader;

	unsigned int m_ComputeShader;

private:
	void copyTextures();

public:
	CellBlock(int blockSize);
	void update();

	unsigned int getCellTexture();
	int getSize();
};
