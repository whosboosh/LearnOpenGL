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
		void draw(glm::mat4 projection, glm::mat4 viewMatrix);

		void DirectionalShadowMapPass(DirectionalLight* light);
		void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
		void RenderScene(Shader* shader);

		int addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		int addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const char* textureName);

		int addModel(const char* fileName, const char* textureName);
		int addModel(const char* fileName);

		void createDirectionalLight(glm::vec3 position, glm::vec3 colour, float ambientIntensity, float diffuseIntensity);
		void updateDirectionalLight(glm::vec3* position, glm::vec3* colour, float* ambientIntensity, float* diffuseIntensity);

		void setShadows(bool state);
		void setTextureStateMesh(int meshIndex, bool state);
		void setTextureStateModel(int modelIndex, bool state);

		void setModelList(std::vector<Model*> modelList);
		void setMeshList(std::vector<Mesh*> meshList);
		void setMultiSampleLevel(int level) { 
			if (level % 2 == 0 || level == 1) {
				this->multiSampleLevel = level;
			};
			multiSampler.init(multiSampleLevel);
		}
		void setSampleShading(float sampleValue) { setSampleShading(sampleValue); };

		void updateModelMatrix(int modelId, glm::mat4 *newModel);
		void updateMeshMatrix(int modelId, glm::mat4 *newModel);

		~OpenGL();

	private:
		Camera* camera;
		Window* window;

		unsigned int quadVAO = 0;
		unsigned int quadVBO;

		int multiSampleLevel = 1;

		std::vector<Model*> modelList;
		std::vector<Mesh*> meshList;

		Shader geometryShader;
		Shader directionalShadowShader;
		Shader screenShader;
		MultiSampler multiSampler;

		DirectionalLight *directionalLight;
		unsigned int VBO;
	};


}


