#pragma once

#include <stdio.h>
#include <glad/glad.h>
#include <iostream>

class GBuffer
{
public:
	GBuffer(GLuint width, GLuint height);

	void init();

	void updateScreenSize(GLuint width, GLuint height);

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

