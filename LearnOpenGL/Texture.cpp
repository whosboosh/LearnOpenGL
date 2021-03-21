#include "Texture.h"


namespace opengl {
	Texture::Texture()
	{
		textureID = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = NULL;
	}


	Texture::Texture(const char* fileLoc)
	{
		textureID = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = fileLoc;
	}

	bool Texture::LoadTexture()
	{
		unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to find: %s\n", fileLocation);
			return false;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(texData);

		return true;
	}

	bool Texture::LoadTextureA()
	{
		unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
		if (!texData)
		{
			printf("Failed to find: %s\n", fileLocation);
			return false;
		}
		glGenTextures(1, &textureID); // Create a texture in memory, assigns the value of the texture to "textureID"
		glBindTexture(GL_TEXTURE_2D, textureID); // Binds this texture to the GL_TEXTURE_2D target

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Attribute configuration for wrapping and filtering...
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Write the image data to the buffer at GL_TEXTURE_2D (currently bound to textureID)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Cleanup
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(texData);

		return true;
	}

	void Texture::UseTexture(GLenum textureUnit)
	{
		// We can have multiple textures bound to a shader at any one time, (Separate textures for RGB textures, Normal Map textures, shadow map textures etc)
		// textureUnit specifies what texture unit we want to bind E.G. (GL_TEXTURE0)
		glActiveTexture(textureUnit); 
		glBindTexture(GL_TEXTURE_2D, textureID); // Bind the texture at textureID to GL_TEXTURE_2D.
	}

	void Texture::RemoveTexture()
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::ClearTexture()
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = NULL;
	}

	Texture::~Texture()
	{
		ClearTexture();
	}
}
