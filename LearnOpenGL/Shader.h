#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GLAD\glad.h>

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

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity,
		uniformDirection, uniformSpecularIntensity, uniformSpecularShininess, uniformEyePositionLocation, uniformInverseTransposeLocation,
		uniformShouldUseTexture;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};