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
		geometryShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.frag");
		directionalShadowShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.frag");
		screenShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.frag");
	}

	void OpenGL::draw(glm::mat4 projectionMatrix)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		RenderPass(projectionMatrix, camera->calculateViewMatrix());
	}

	void OpenGL::RenderScene(Shader* shader)
	{
		for (int i = 0; i < meshList.size(); i++)
		{
			glm::mat4 model(1.0f); // Identity matrix
			model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
			shader->setMat4("model", model);
			shader->setMat4("inverseTransposeModel", glm::transpose(glm::inverse(model)));
			meshList[i]->RenderMeshIndex(shader);
			//glm::vec3 cameraPos = camera->getCameraPosition();
			//std::cout << "Camera : " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << "\n";
		}
	}

	void OpenGL::RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
	{
		geometryShader.UseShader();

		glViewport(0, 0, window->getBufferWidth(), window->getBufferHeight());

		geometryShader.setMat4("projection", projectionMatrix);
		geometryShader.setMat4("view", viewMatrix);
		//geometryShader.setVec3("eyePosition", glm::vec3(camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z));
		//geometryShader.setInt("theTexture", 0); //GL_TEXTURE0

		//mainLight.GetShadowMap()->Read(GL_TEXTURE1);
		//geometryShader.setInt("directionalShadowMap", 1);
		//geometryShader.setInt("normalMap", 2);

		//geometryShader.SetDirectionalLight(&mainLight);
		//geometryShader.SetPointLights(pointLights, pointLightCount);

		//glm::mat4 lightTransform = mainLight.CalculateLightTransform();
		//geometryShader.setMat4("directionalLightTransform", lightTransform);

		RenderScene(&geometryShader);
	}

	void OpenGL::addMesh(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices)
	{
		// Create a mesh
		opengl::Mesh* mesh = new opengl::Mesh();
		mesh->CreateMeshIndex(vertices, indices);
		meshList.push_back(mesh);
	}

	void OpenGL::setModelList(std::vector<Model*> modelList)
	{
		this->modelList = modelList;
	}

	void OpenGL::setMeshList(std::vector<Mesh*> meshList)
	{
		this->meshList = meshList;
	}


	OpenGL::~OpenGL()
	{
		//delete meshList;
		//delete modelList;
	}
}
