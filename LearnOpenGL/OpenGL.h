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

#include <iostream>

namespace opengl {
	class OpenGL
	{
	public:
		OpenGL();

		void init(Window* window, Camera* camera);
		void draw();

		void setModelList(std::vector<Model> *modelList);
		void setMeshList(std::vector<Mesh> *meshList);

		~OpenGL();

	private:
		Camera* camera;
		Window* window;

		std::vector<Model> *modelList;
		std::vector<Mesh> *meshList;

		Shader geometryShader;
		Shader directionalShadowShader;
		Shader screenShader;
		MultiSampler multiSampler;
	};

}


