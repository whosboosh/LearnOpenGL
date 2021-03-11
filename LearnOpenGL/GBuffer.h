#pragma once

#include <stdio.h>
#include <glad/glad.h>
#include <iostream>

namespace opengl {
	class GBuffer
	{
	public:
		GBuffer();
		GBuffer(GLuint width, GLuint height);

		void init();
		void updateScreenSize(GLuint width, GLuint height);
		void BindFrameBuffer();
		void ReadFrameBuffer();
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
}