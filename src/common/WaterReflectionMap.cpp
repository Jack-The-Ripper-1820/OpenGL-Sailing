#include "WaterReflectionMap.hpp"

WaterReflectionMap::WaterReflectionMap()
{
	FBO = 0;
	waterReflectionMap = 0;
}

bool WaterReflectionMap::Init(unsigned int width, unsigned int height)
{
	waterReflectionWidth = width; waterReflectionHeight = height;

	glGenFramebuffers(1, &FBO);

	// Create WaterReflection texture
	glGenTextures(1, &waterReflectionMap);
	glBindTexture(GL_TEXTURE_2D, waterReflectionMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // change this for edge clamping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, waterReflectionMap, 0);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer error: %s\n", Status);
		return false;
	}

	return true;
}

void WaterReflectionMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void WaterReflectionMap::Read(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, waterReflectionMap);
}

WaterReflectionMap::~WaterReflectionMap()
{
	if (FBO) {
		glDeleteFramebuffers(1, &FBO);
	}

	if (waterReflectionMap) {
		glDeleteTextures(1, &waterReflectionMap);
	}
}
