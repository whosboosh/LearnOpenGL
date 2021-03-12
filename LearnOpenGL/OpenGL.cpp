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
		std::string prefix = "../../LearnOpenGL/LearnOpenGL/Shaders";
		geometryShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/geometry_shader.frag");
		directionalShadowShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/directional_shadow_map.frag");
		screenShader.CreateFromFiles("../../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.vert", "../../LearnOpenGL/LearnOpenGL/Shaders/anti_aliasing.frag");


	}

	void OpenGL::draw()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGL::setModelList(std::vector<Model> *modelList)
	{
		this->modelList = modelList;
	}

	void OpenGL::setMeshList(std::vector<Mesh>* meshList)
	{
		this->meshList = meshList;
	}


	OpenGL::~OpenGL()
	{
	
	}
}
