#include "Model.h"

Model::Model()
{
}

Model::Model(const std::string& fileName)
{
	this->fileName = fileName;
	this->customTexture = nullptr;
}

Model::Model(const std::string& fileName, Texture* customTexture) 
{
	this->fileName = fileName;
	this->customTexture = customTexture;
}

void Model::LoadModel()
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		std::cout << "Failed to load model (" << fileName << "): " << importer.GetErrorString() << "\n";
	}
	
	LoadMaterials(scene);
	LoadNode(scene->mRootNode, scene);
}

void Model::RenderModel(Shader* shader)
{
	for (size_t i = 0; i < meshList.size(); i++) {
		unsigned int materialIndex = meshToTex[i];

		//if (materialIndex < textureList.size() && textureList[materialIndex]) {
			//textureList[materialIndex]->UseTexture(GL_TEXTURE0);
		//}

		meshList[i]->RenderMeshIndex(shader);
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}
	/*
	if (customTexture) {
		delete customTexture;
	}
	delete standardMaterial;

	for (size_t i = 0; i < textureList.size(); i++) {
		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
	*/

}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], i, scene);
	}
	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}

}

void Model::LoadMesh(aiMesh* mesh, unsigned int meshIndex, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.resize(mesh->mNumVertices);
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		// Set position
		vertices[i].pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

		// Set texture coords if they exist
		if (mesh->mTextureCoords[0]) {
			vertices[i].tex = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		}
		else {
			vertices[i].tex = { 0.0f, 0.0f };
		}
		vertices[i].col = { 1.0f, 1.0f, 1.0f };
		vertices[i].normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

		//std::cout << vertices[i].normal.x << " " << vertices[i].normal.y << " " << vertices[i].normal.z;
	}

	// Go through faces indicies and add to list
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		// Get face
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	meshToTex.push_back(mesh->mMaterialIndex);
	Mesh* newMesh = new Mesh(textureList[meshIndex], standardMaterial);
	newMesh->CreateMeshIndex(&vertices, &indices);
	meshList.push_back(newMesh);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Get a fileName from the last backslash (remove absolute path)
				int index = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(index + 1);

				std::string texPath = std::string("Textures/") + fileName;
				textureList[i] = new Texture(texPath.c_str());



				if (!textureList[i]->LoadTexture())
				{
					// If texture fails, set it to null
					std::cout << "Failed to load texture at: " << texPath << " \n";
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i] && !customTexture) {
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureA();
		}
		else {
			textureList[i] = customTexture;
			textureList[i]->LoadTexture();
		}
	}
}

Model::~Model()
{
	ClearModel();
}