#include "OpenGL.h"

namespace opengl {
	OpenGL::OpenGL()
	{

	}
	void OpenGL::init(Window* window, Camera* camera)
	{
		this->window = window;
		this->camera = camera;

		// Create multisampler
		multiSampler = MultiSampler(window->getBufferWidth(), window->getBufferHeight());
		multiSampler.init(multiSampleLevel);

		// Create shaders
		geometryShader.CreateFromFiles("../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.vert", "../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.frag");
		directionalShadowShader.CreateFromFiles("../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.vert", "../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.frag");
		screenShader.CreateFromFiles("../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.vert", "../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.frag");

		directionalLight = new DirectionalLight();

		float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};
		// setup screen VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	void OpenGL::draw(glm::mat4 projection, glm::mat4 viewMatrix)
	{
		multiSampler.BindFrameBuffer();

		glClearColor(0.6f, 0.65f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		DirectionalShadowMapPass(directionalLight);

		multiSampler.BindFrameBuffer();

		RenderPass(projection, viewMatrix);
		
		multiSampler.BlitFrameBuffer();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		screenShader.UseShader();
		glBindVertexArray(quadVAO);
		multiSampler.Read(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void OpenGL::RenderScene(Shader* shader)
	{
		for (int i = 0; i < meshList.size(); i++)
		{
			shader->setMat4("model", meshList[i]->getModel());
			shader->setMat4("inverseTransposeModel", glm::transpose(glm::inverse(meshList[i]->getModel())));
			meshList[i]->RenderMeshIndex(shader);
			//glm::vec3 cameraPos = camera->getCameraPosition();
			//std::cout << "Camera : " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << "\n";
		}
		for (int i = 0; i < modelList.size(); i++)
		{
			shader->setMat4("model", modelList[i]->getModel());
			shader->setMat4("inverseTransposeModel", glm::transpose(glm::inverse(modelList[i]->getModel())));
			modelList[i]->RenderModel(shader);
		}
	}

	void OpenGL::DirectionalShadowMapPass(DirectionalLight* light)
	{
		directionalShadowShader.UseShader();

		glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

		// Calls glBindFrameBuffer(GL_FRAMEBUFFER, FBO) in ShadowMap
		light->GetShadowMap()->BindFrameBuffer();
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightTransform = light->CalculateLightTransform();
		directionalShadowShader.setMat4("directionalLightTransform", lightTransform);

		RenderScene(&directionalShadowShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGL::RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	{
		geometryShader.UseShader();

		glViewport(0, 0, window->getBufferWidth(), window->getBufferHeight());

		geometryShader.setMat4("projection", projectionMatrix);
		geometryShader.setMat4("view", viewMatrix);
		geometryShader.setVec3("eyePosition", glm::vec3(camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z));
		geometryShader.setInt("theTexture", 0); //GL_TEXTURE0

		directionalLight->GetShadowMap()->Read(GL_TEXTURE1);
		geometryShader.setInt("directionalShadowMap", 1);
		geometryShader.setInt("normalMap", 2);

		geometryShader.SetDirectionalLight(directionalLight);
		//geometryShader.SetPointLights(pointLights, pointLightCount);

		glm::mat4 lightTransform = directionalLight->CalculateLightTransform();
		geometryShader.setMat4("directionalLightTransform", lightTransform);

		RenderScene(&geometryShader);
	}

	int OpenGL::addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		// Create a mesh
		Mesh* mesh = new Mesh(vertices, indices);
		mesh->CreateMeshIndex();
		meshList.push_back(mesh);

		return meshList.size() - 1;
	}


	int OpenGL::addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const char* textureName)
	{
		Texture *texture = new Texture(textureName);
		std::string s = textureName;
		std::string extension = s.substr(s.find_last_of("."));
		if (extension.find("png") != std::string::npos)
		{
			texture->LoadTexture();
		}
		else {
			texture->LoadTexture();
		}

		Material *standardMaterial = new Material(0.8f, 64);

		Mesh* mesh = new Mesh(vertices, indices, texture, standardMaterial);
		mesh->CreateMeshIndex();
		meshList.push_back(mesh);

		return meshList.size() - 1;
	}

	int OpenGL::addModel(const char* fileName, const char* textureName)
	{ 
		// Create a mesh
		Model *model = new Model(fileName, new Texture(textureName));
		model->LoadModel();
		modelList.push_back(model);

		return modelList.size() - 1;
	}

	int OpenGL::addModel(const char* fileName)
	{
		Model* model = new Model(fileName);
		model->LoadModel();
		modelList.push_back(model);

		return modelList.size() - 1;
	}

	void OpenGL::createDirectionalLight(glm::vec3 position, glm::vec3 colour, float ambientIntensity, float diffuseIntensity)
	{
		directionalLight = new DirectionalLight(8192, 8192, colour.x, colour.y, colour.z, ambientIntensity, diffuseIntensity, position.x, position.y, position.z);
	}

	void OpenGL::updateDirectionalLight(glm::vec3* position, glm::vec3* colour, float* ambientIntensity, float* diffuseIntensity)
	{
		directionalLight->updateLight(position, colour, ambientIntensity, diffuseIntensity);
	}

	void OpenGL::setShadows(bool state)
	{
		std::cout << state;
		geometryShader.UseShader();
		geometryShader.setBool("shouldUseShadows", state);
	}

	void OpenGL::setTextureStateMesh(int meshIndex, bool state)
	{
		if (meshIndex > meshList.size() -1) return;
		meshList[meshIndex]->setShouldUseTexture(state);
	}

	void OpenGL::setTextureStateModel(int modelIndex, bool state)
	{
		if (modelIndex > modelList.size() - 1) return;
		for (int i = 0; i < modelList[modelIndex]->getMeshList().size(); i++)
		{
			modelList[modelIndex]->getMeshList()[i]->setShouldUseTexture(state);
		}

	}

	void OpenGL::setModelList(std::vector<Model*> modelList)
	{
		this->modelList = modelList;
	}

	void OpenGL::setMeshList(std::vector<Mesh*> meshList)
	{
		this->meshList = meshList;
	}

	void OpenGL::updateModelMatrix(int modelId, glm::mat4 *newModel)
	{
		if (modelId >= modelList.size()) return;
		modelList[modelId]->setModel(*newModel);
	}

	void OpenGL::updateMeshMatrix(int modelId, glm::mat4 *newModel)
	{
		if (modelId >= meshList.size()) return;
		meshList[modelId]->setModel(*newModel);
	}


	OpenGL::~OpenGL()
	{
		for (int i = 0; i < meshList.size(); i++)
		{
			delete meshList[i];
		}
		for (int i = 0; i < modelList.size(); i++)
		{
			delete modelList[i];
		}

		delete directionalLight;
	}
}
