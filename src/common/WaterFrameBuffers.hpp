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

	void InitReflectionFrameBuffer();
	void InitRefractionFrameBuffer();

public:
	WaterFrameBuffers();
	~WaterFrameBuffers();

	void ReadTextures();

	void BindReflectionFrameBuffer();
	void BindRefractionFrameBuffer();

	void UnbindCurrentFrameBuffer();

	GLuint GetReflectionTexture() { return reflectionTexture; }
	GLuint GetRefractionTexture() { return refractionTexture; }

	void BindFrameBuffer(GLuint frameBuffer, int width, int height);

	GLuint CreateFrameBuffer();

	GLuint CreateTextureAttachment(int width, int height);
	GLuint CreateDepthTextureAttachment(int width, int height);
	GLuint CreateDepthBufferAttachment(int width, int height);
};