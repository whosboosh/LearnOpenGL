#include "Mesh.h"

namespace opengl {
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Texture* texture, Material* material)
	{
		this->vertices = vertices;
		this->indices = indices;

		this->texture = texture;
		this->material = material;
	}

	void Mesh::CreateMeshIndex()
	{

		this->indiceCount = indices.size();
		this->verticeCount = vertices.size();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indiceCount, indices.data(), GL_DYNAMIC_DRAW);
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * verticeCount, &vertices.at(0).pos.x, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void Mesh::RenderMeshIndex(Shader* shader)
	{
		shader->setBool("shouldUseTexture", 1);
		if (texture && material) {
			texture->UseTexture(GL_TEXTURE0);
			material->UseMaterial(shader);
		}
		else {
			shader->setBool("shouldUseTexture", 0);
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Mesh::RenderMesh(unsigned int texture)
	{
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Mesh::ClearMesh()
	{
		if (IBO != 0)
		{
			glDeleteBuffers(1, &IBO);
			IBO = 0;
		}
		if (VBO != 0)
		{
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (VAO != 0)
		{
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		indiceCount = 0;
	}

	Mesh::~Mesh()
	{
		ClearMesh();

		delete material;
		delete texture;
	}
}
