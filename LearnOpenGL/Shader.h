#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "DirectionalLight.h"
#include "PointLight.h"

#include <GLAD\glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

const int MAX_POINT_LIGHTS = 3;

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetSpecularShininessLocation();
	GLuint GetEyePositionLocation();
	GLuint GetInverseTransposeModelLocation();
	GLuint GetUniformShouldUseTextureLocation();
	GLuint GetUniformNormalMapLocation();
	GLuint GetUniformShouldUseNormalMapLocation();

	void SetDirectionalLight(DirectionalLight *dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);

	void SetTexture(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetNormalMap(GLuint textureUnit);
	void SetGPosition(GLuint textureUnit);
	void SetGNormal(GLuint textureUnit);
	void SetGAlbedo(GLuint textureUnit);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint uniformPointLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		uniformSpecularIntensity, uniformSpecularShininess, uniformEyePosition, uniformInverseTranspose,
		uniformShouldUseTexture, uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformTexture, uniformNormalMap, uniformShouldUseNormalMap, uniformGPosition, uniformGAlbedoSpec, uniformGNormal;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};