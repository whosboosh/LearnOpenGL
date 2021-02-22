#pragma once

#include <stdio.h>
#include <glad/glad.h>
#include <iostream>

class GBuffer
{
public:
	GBuffer();
	GBuffer(GLuint width, GLuint height);

	void init();
	void updateScreenSize(GLuint width, GLuint height);
	void Write();
	void Read(GLenum textureUnit, char* type);

	~GBuffer();

private:
	GLuint gBuffer;
	GLuint rboDepth;

	GLuint gPosition;
	GLuint gNormal;
	GLuint gAlbedoSpec;

	GLuint screenWidth;
	GLuint screenHeight;
};

