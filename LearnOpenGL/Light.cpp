#include "Light.h"

namespace opengl {
	Light::Light()
	{
		shadowMap = new ShadowMap();
		shadowMap->init(8192, 8192);

		colour = glm::vec3(1.0f, 1.0f, 1.0f);
		ambientIntensity = 1.0f;

		diffuseIntensity = 0.0f;
	}

	Light::Light(GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
	{
		shadowMap = new ShadowMap();
		shadowMap->init(shadowWidth, shadowHeight);

		colour = glm::vec3(red, green, blue);
		ambientIntensity = aIntensity;

		diffuseIntensity = dIntensity;
	}

	Light::~Light()
	{
	}

}

