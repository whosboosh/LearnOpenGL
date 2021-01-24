#pragma once

#include "GLAD/glad.h"
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	void LoadTexture();
	void UseTexture(GLenum textureUnit);
	void ClearTexture();
	void RemoveTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char *fileLocation;
};

