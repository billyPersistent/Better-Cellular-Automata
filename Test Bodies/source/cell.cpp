#include "cell.h"

void CellBlock::copyTextures()
{
	//bind textures and copy shader
	glUseProgram(m_CopyShader);
	glBindImageTexture(0, m_CellTextureRead, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	glBindImageTexture(1, m_CellTextureWrite, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA32F);

	//set image uniforms
	int imageReadLocation = glGetUniformLocation(m_CopyShader, "u_CellTextureRead");
	glUniform1i(imageReadLocation, 0);

	int imageWriteLocation = glGetUniformLocation(m_CopyShader, "u_CellTextureWrite");
	glUniform1i(imageWriteLocation, 1);

	//copy write texture into read texture
	glDispatchCompute(1, m_BlockSize, m_BlockSize);
}

//populate big vertex array of cells
CellBlock::CellBlock(int blockSize)
	:m_BlockSize(blockSize)
{
	//cell textures
	m_CellTextureRead = createCellTexture(blockSize);
	m_CellTextureWrite = createCellTexture(blockSize);

	//texture copying compute shader
	m_CopyShader = createShader(loadShader("shaders/copyc.shader"));

	//give write texture a white texel for debug
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, m_CellTextureWrite);

	glm::vec4 fuckler(1.0f, 1.0f, 1.0f, 1.0f);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 50, 50, 50, 1, 1, 1, GL_RGBA, GL_FLOAT, &fuckler);
	//glTexSubImage3D(GL_TEXTURE_3D, 0, 50, 51, 50, 1, 1, 1, GL_RGBA, GL_FLOAT, &fuckler);
	//glTexSubImage3D(GL_TEXTURE_3D, 0, 50, 49, 50, 1, 1, 1, GL_RGBA, GL_FLOAT, &fuckler);

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, m_CellTextureWrite);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 49, 10, 10, 1, 1, 1, GL_RGBA, GL_FLOAT, &fuckler);*/

	//copy read texture into write texture (so they're the same)
	copyTextures();

	//create standin simulation shader
	m_ComputeShader = createShader(loadShader("shaders/cellc.shader"));
}

void CellBlock::update()
{
	//bind texture and shader
	glUseProgram(m_ComputeShader);
	glBindImageTexture(0, m_CellTextureRead, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, m_CellTextureWrite, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	//set image uniforms
	int imageReadLocation = glGetUniformLocation(m_ComputeShader, "u_CellTextureRead");
	glUniform1i(imageReadLocation, 0);

	int imageWriteLocation = glGetUniformLocation(m_ComputeShader, "u_CellTextureWrite");
	glUniform1i(imageWriteLocation, 1);

	//compute
	glDispatchCompute(1, m_BlockSize, m_BlockSize);

	//this makes sure the texture can actually be read right (getit?)
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);//

	//copy read texture into write texture (so they're the same)
	copyTextures();
}

//return boring stuff

unsigned int CellBlock::getCellTexture()
{
	return m_CellTextureRead;
}

int CellBlock::getSize()
{
	return m_BlockSize;
}