#include "Material.h"

namespace opengl {
	Material::Material()
	{
		specularIntensity = 1.0f;
		shininess = 1.0f;
	}

	Material::Material(GLfloat sIntensity, GLfloat shine)
	{
		specularIntensity = sIntensity;
		shininess = shine;
	}

	void Material::UseMaterial(Shader* shader)
	{
		shader->setInt("material.specularIntensity", specularIntensity);
		shader->setInt("material.shininess", shininess);
	}

	Material::~Material()
	{
	}

}

