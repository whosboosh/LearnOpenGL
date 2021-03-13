#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <vector>
#include <string>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace opengl {
	class Model
	{
	public:
		Model();
		Model(const std::string& fileName);
		Model(const std::string& fileName, Texture* customTexture);

		void setModel(glm::mat4 model) { this->model = model; }
		glm::mat4 getModel() { return model; }

		void LoadModel();
		void RenderModel(Shader* shader);
		void ClearModel();

		~Model();
	private:
		// Model data
		std::vector<Mesh*> meshList;
		std::vector<Texture*> textureList;
		std::vector<unsigned int> meshToTex;
		
		glm::mat4 model;

		std::string fileName;
		Texture* customTexture;
		Material* standardMaterial = new Material(0.6f, 64);

		void LoadNode(aiNode* node, const aiScene* scene);
		void LoadMesh(aiMesh* mesh, unsigned int meshIndex, const aiScene* scene);
		void LoadMaterials(const aiScene* scene);
	};


}