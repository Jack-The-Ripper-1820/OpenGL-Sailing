#pragma once

#include <stdio.h>
#include <GL\glew.h>
#include <iostream>

class WaterFrameBuffers {
private:
	const static int REFLECTION_WIDTH = 320;
	const static int REFLECTION_HEIGHT = 180;

	const static int REFRACTION_WIDTH = 1280;
	const static int REFRACTION_HEIGHT = 720;

	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture; 


public:
	WaterFrameBuffers();



};