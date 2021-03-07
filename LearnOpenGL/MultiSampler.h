#pragma once

#include <stdio.h>
#include <iostream>
#include <glad/glad.h>

class MultiSampler
{
public:
	MultiSampler();
	MultiSampler(GLuint width, GLuint height);

	virtual void init();
	virtual void BlitFrameBuffer();
	virtual void BindFrameBuffer();
	virtual void Read(GLenum textureUnit);

	void updateScreenSize(GLuint width, GLuint height) {
		screenHeight = height;
		screenWidth = width;
	}

	~MultiSampler();

private:


protected:
	GLuint framebuffer;
	GLuint textureColourBufferMultiSampled; // Mutlisampled colour attachment texture
	GLuint rbo; // Render buffer object for depth and stencil attachments

	GLuint intermediateFBO; // Second framebuffer for post-processing
	GLuint screenTexture;

	GLuint screenWidth;
	GLuint screenHeight;
};

