#pragma once

#include "Shader.h"
#include <vector>
#include "Mesh.h"

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

		void LoadModel();
		void RenderModel(Shader* shader);
		void ClearModel();

		~Model();
	private:
		// Model data
		std::vector<Mesh*> meshList;
		std::vector<Texture*> textureList;
		std::vector<unsigned int> meshToTex;

		std::string fileName;
		Texture* customTexture;

		Material* standardMaterial = new Material(0.6f, 64);

		void LoadNode(aiNode* node, const aiScene* scene);
		void LoadMesh(aiMesh* mesh, unsigned int meshIndex, const aiScene* scene);
		void LoadMaterials(const aiScene* scene);
	};


}