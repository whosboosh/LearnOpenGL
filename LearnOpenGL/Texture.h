#pragma once

#include <GLAD\glad.h>
#include "stb_image.h"

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

