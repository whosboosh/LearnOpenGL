#pragma once

#include <stdio.h>
#include <iostream>
#include <glad/glad.h>

namespace opengl {
	class MultiSampler
	{
	public:
		MultiSampler();
		MultiSampler(GLuint width, GLuint height);

		virtual void init(int multiSampleLevel);
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
		GLuint textureColorBufferMultiSampled; // Mutlisampled colour attachment texture
		GLuint rbo; // Render buffer object for depth and stencil attachments
		GLuint intermediateFBO;
		GLuint screenTexture;

		GLuint screenWidth;
		GLuint screenHeight;
	};


}