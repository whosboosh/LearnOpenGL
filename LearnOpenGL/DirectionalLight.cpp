#include "DirectionalLight.h"

namespace opengl {
	DirectionalLight::DirectionalLight() : Light()
	{
		direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
	}

	DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
	{
		direction = glm::vec3(xDir, yDir, zDir);
		lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
	}

	void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation)
	{
		glUniform3fv(ambientColourLocation, 1, &colour[0]);
		glUniform3fv(directionLocation, 1, &direction[0]);

		glUniform1f(ambientIntensityLocation, ambientIntensity);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}

	void DirectionalLight::updateLight(glm::vec3* position, glm::vec3* colour, float* ambientIntensity, float* diffuseIntensity)
	{
		this->direction = *position;
		this->colour = *colour;
		this->ambientIntensity = *ambientIntensity;
		this->diffuseIntensity = *diffuseIntensity;

	}

	glm::mat4 DirectionalLight::CalculateLightTransform()
	{
		return lightProj * glm::lookAt(direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	DirectionalLight::~DirectionalLight()
	{
	}

}

