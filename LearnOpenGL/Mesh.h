#pragma once

#include <GLAD\glad.h>
#include <vector>
#include "Texture.h"
#include "Material.h"

#include "Utilities.h"

namespace opengl {
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* texture, Material* material);
		void CreateMeshIndex();
		void RenderMesh(unsigned int texture);
		void RenderMeshIndex(Shader* shader);
		void ClearMesh();

		void setShouldUseTexture(bool state) { this->shouldUseTexture = state; }
		void setModel(glm::mat4 model) { this->model = model; }
		glm::mat4 getModel() { return model; }

		~Mesh();
	private:
		unsigned int VAO, VBO, IBO;
		unsigned int indiceCount;
		unsigned int verticeCount;

		bool shouldUseTexture;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		glm::mat4 model;

		Texture* texture = nullptr;
		Material* material = nullptr;
	};
}
