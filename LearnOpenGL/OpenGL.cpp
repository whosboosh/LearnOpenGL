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
		multiSampler.init();

		// Create shaders
		geometryShader.CreateFromFiles("../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.vert", "../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.frag");
		//directionalShadowShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.frag");
		//screenShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.frag");

		directionalLight = new DirectionalLight();
	}

	void OpenGL::draw(glm::mat4 projection, glm::mat4 viewMatrix)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		RenderPass(projection, viewMatrix);
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

	void OpenGL::RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	{
		geometryShader.UseShader();

		glViewport(0, 0, window->getBufferWidth(), window->getBufferHeight());

		geometryShader.setMat4("projection", projectionMatrix);
		geometryShader.setMat4("view", viewMatrix);
		//geometryShader.setVec3("eyePosition", glm::vec3(camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z));
		geometryShader.setInt("theTexture", 0); //GL_TEXTURE0

		//mainLight.GetShadowMap()->Read(GL_TEXTURE1);
		//geometryShader.setInt("directionalShadowMap", 1);
		//geometryShader.setInt("normalMap", 2);

		geometryShader.SetDirectionalLight(directionalLight);
		//geometryShader.SetPointLights(pointLights, pointLightCount);

		//glm::mat4 lightTransform = mainLight.CalculateLightTransform();
		//geometryShader.setMat4("directionalLightTransform", lightTransform);

		RenderScene(&geometryShader);
	}

	void OpenGL::rebindObjects()
	{
		for (int i = 0; i < meshList.size(); i++)
		{
			meshList[i]->CreateMeshIndex();
		}
		for (int i = 0; i < modelList.size(); i++)
		{
			// TODO: Add rebind methods for model
			// CreateMeshIndex
		}
	}

	void OpenGL::addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		// Create a mesh
		Mesh* mesh = new Mesh(vertices, indices);
		mesh->CreateMeshIndex();
		meshList.push_back(mesh);
	}


	void OpenGL::addMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const char* textureName)
	{
		Texture *texture = new Texture(textureName);
		texture->LoadTexture();

		Material *standardMaterial = new Material(0.8f, 64);

		Mesh* mesh = new Mesh(vertices, indices, texture, standardMaterial);
		mesh->CreateMeshIndex();
		meshList.push_back(mesh);
	}

	void OpenGL::addModel(const char* fileName, const char* textureName)
	{ 
		// Create a mesh
		Model *model = new Model(fileName, new Texture(textureName));
		model->LoadModel();
		modelList.push_back(model);
	}

	void OpenGL::addModel(const char* fileName)
	{
		Model* model = new Model(fileName);
		model->LoadModel();
		modelList.push_back(model);
	}

	void OpenGL::createDirectionalLight(glm::vec3 position, glm::vec3 colour, float ambientIntensity, float diffuseIntensity)
	{
		directionalLight = new DirectionalLight(8192, 8192, colour.x, colour.y, colour.z, ambientIntensity, diffuseIntensity, position.x, position.y, position.z);
	}

	void OpenGL::updateDirectionalLight(glm::vec3* position, glm::vec3* colour, float* ambientIntensity, float* diffuseIntensity)
	{
		directionalLight->updateLight(position, colour, ambientIntensity, diffuseIntensity);
	}

	void OpenGL::setModelList(std::vector<Model*> modelList)
	{
		this->modelList = modelList;
	}

	void OpenGL::setMeshList(std::vector<Mesh*> meshList)
	{
		this->meshList = meshList;
	}

	void OpenGL::updateModelMatrix(int modelId, glm::mat4 newModel)
	{
		if (modelId >= modelList.size()) return;
		modelList[modelId]->setModel(newModel);
	}

	void OpenGL::updateMeshMatrix(int modelId, glm::mat4 newModel)
	{
		if (modelId >= meshList.size()) return;
		meshList[modelId]->setModel(newModel);
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
