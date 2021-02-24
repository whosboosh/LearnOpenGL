#pragma once

#include <stdio.h>
#include "GLAD/glad.h"

class ShadowMap
{
public:
	ShadowMap();

	virtual bool init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);

	void updateShadowSize(GLuint width, GLuint height) { shadowWidth = width; shadowHeight = height; }
	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }

	~ShadowMap();

private:


protected:
	GLuint FBO, shadowMap;
	GLuint shadowWidth, shadowHeight;
};

