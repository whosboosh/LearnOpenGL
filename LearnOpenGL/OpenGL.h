#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "GBuffer.h"
#include "Utilities.h"
#include "Window.h"

namespace opengl {
	class OpenGL
	{
	public:
		OpenGL(unsigned int width, unsigned int height);

		Window* getWindow() { return window; }
		Camera* getCamera() { return camera; }

		void CameraControl(float deltaTime);

		void RenderScene(Shader* shader);
		void DirectionalShadowPass();
		void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);

		void UpdateObjects(std::vector<Vertex> meshList, DirectionalLight light, std::vector<PointLight> pointLights);

		~OpenGL();

	private:
		Window* window;
		Camera* camera;

		Shader geometryShader;
		Shader directionalShadowShader;
		Shader screenShader;

		DirectionalLight* mainLight;
		std::vector<Vertex> meshList;
		PointLight pointLights[];
	};

}