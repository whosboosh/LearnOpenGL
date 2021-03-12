#include "Mesh.h"
#include "Shader.h"
#include "../../API Wrapper/Window.h"
#include "../../API Wrapper/Camera.h"
#include "Texture.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "GBuffer.h"
#include "Utilities.h"
#include "MultiSampler.h"
#include "Model.h"
#include "Mesh.h"

#include <iostream>
#include <string>

namespace opengl {
	class OpenGL
	{
	public:
		OpenGL();

		void init(Window* window, Camera* camera);
		void draw();

		void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
		void RenderScene(Shader* shader);

		void addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		void addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const char* textureName);

		void addModel(const char* fileName, const char* textureName);
		void addModel(const char* fileName);

		void rebindObjects();

		void setModelList(std::vector<Model*> modelList);
		void setMeshList(std::vector<Mesh*> meshList);

		void updateModelMatrix(int modelId, glm::mat4 newModel);
		void updateMeshMatrix(int modelId, glm::mat4 newModel);

		~OpenGL();

	private:
		Camera* camera;
		Window* window;

		std::vector<Model*> modelList;
		std::vector<Mesh*> meshList;

		Shader geometryShader;
		Shader directionalShadowShader;
		Shader screenShader;
		MultiSampler multiSampler;
	};

}


