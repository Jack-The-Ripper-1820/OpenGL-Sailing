#pragma once

#include <GL\glew.h>
#include <stb_image.h>

class Texture {
public:
	Texture();
	Texture(char const* fileLocation);
	~Texture();

	bool LoadTexture();
	void UseTexture();
	void ClearTexture();
	bool LoadTextureA();
	
private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;

};