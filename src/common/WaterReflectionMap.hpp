#pragma once

#include <stdio.h>
#include <GL\glew.h>
#include <iostream>

class WaterReflectionMap {
public:
	WaterReflectionMap();

	virtual bool Init(unsigned int width, unsigned height);

	virtual void Write();

	virtual void Read(GLenum textureUnit);

	GLuint GetWaterReflectionWidth() { return waterReflectionWidth; }
	GLuint GetWaterReflectionHeight() { return waterReflectionHeight; }

	~WaterReflectionMap();

protected:
	GLuint FBO, waterReflectionMap;
	GLuint waterReflectionWidth, waterReflectionHeight;
};