#pragma once

#include <GLAD\glad.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

namespace opengl {
	class Texture
	{
	public:
		Texture();
		Texture(const char* fileLoc);

		bool LoadTexture();
		bool LoadTextureA();
		void UseTexture(GLenum textureUnit);
		void ClearTexture();
		void RemoveTexture();

		~Texture();

	private:
		GLuint textureID;
		int width, height, bitDepth;

		const char* fileLocation;
	};


}

