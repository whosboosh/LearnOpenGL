#define STB_IMAGE_IMPLEMENTATION

#include <GLAD\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stdio.h>
#include <iostream>

#include "Mesh.h"
#include "Shader.h"
#include "main.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "GBuffer.h"
#include "Utilities.h"
#include "MultiSampler.h"

unsigned int pointLightCount = 0;

float xOffset = 0.0f;
float yOffset = 6.0f;
float zOffset = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;

Shader geometryShader;
Shader directionalShadowShader;
Shader screenShader;

Camera camera;

Texture plainTexture;

Texture brickDiffuse;
Texture brickNormal;
Texture brickDisplacement;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

const float toRadians = 3.14159265f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 2.0f;
float triIncrement = 0.5f;

float curAngle = 0.0f;

double deltaTime = 0.0f;
double lastTime = 0.0f;

float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
bool sizeDirection = false;

unsigned int quadVAO, quadVBO;

void CreateObjects() {
	std::vector<Vertex> vertices = {
			{ { -1.0, -1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 0.0 }}, // 0
			{ { 1.0, -1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 0.0 }},
			{ { -1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 0.0 }},
			{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, 0.0 }},


			{ { 1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 0.0 }}, // 4
			{ { 1.0, -1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 0.0 }},
			{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 0.0 }},
			{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, 0.0 }},

			{ { 1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 0.0 }}, // 8 back
			{ { -1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 0.0 }},
			{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, 0.0 }},
			{ { -1.0, 1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 0.0 }},

			{ { -1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 0.0 }}, // 12 left
			{ { -1.0, -1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 0.0 }},
			{ { -1.0, 1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 0.0 }},
			{ { -1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, 0.0 }},

			{ { -1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 0.0 }}, // 16
			{ { 1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 0.0 }},
			{ { -1.0, -1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, 0.0 }},
			{ { 1.0, -1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 0.0 }},

			{ { -1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 0.0 }}, // 20
			{ { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 1.0 }, { 0.0, 0.0, 0.0 }},
			{ { -1.0, 1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 0.0 }},
			{ { 1.0, 1.0, -1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0 }, { 0.0, 0.0, 0.0 }},
	};

	unsigned int indices[] = {
		2,3,1, 1,0,2,  //Face front
		6,7,4, 4,5,6, //Face right
		9,8,10, 10,11,9, // Back
		13,12,14, 14,15,13, // Left
		19,17,16, 16,18,19, // Bottom
		20,22,23, 23,21,20, // Top
	};

	std::vector<Vertex> vertices2 = {
		{ { -1.0, -1.0, 0.0 }, {1.0, 1.0, 1.0 }, { 0.0, 0.0 }, { 0.0, 0.0, 0.0 }},
		{ { 0.0, -1.0, 1.0 }, {1.0, 1.0, 1.0 }, { 0.5, 0.0 }, { 0.0, 0.0, 0.0 }},
		{ { 1.0, -1.0, 0.0 }, {1.0, 1.0, 1.0 }, { 1.0, 0.0 }, { 0.0, 0.0, 0.0 }},
		{ { 0.0, 1.0, 0.0 }, {1.0, 1.0, 1.0 }, { 0.5, 1.0 }, { 0.0, 0.0, 0.0 }},
	};

	unsigned int indices2[] = {
		0,3,1,
		1,2,3,
		2,3,0,
		0,1,2
	};


	std::vector<Vertex> floorVertices = {
		{ { -40, 0, -40}, { 0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f} }, //BL
		{ { 40, 0, -40}, { 10.0f, 0.0f, 0.0f}, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f} },//BR
		{ { -40, 0, 40 }, { 0.0f, 10.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f} },//FL
		{ { 40, 0, 40 }, { 10.0f, 10.0f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f} }//FR
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	calcAverageNormals(indices, &vertices, sizeof(indices) / sizeof(*indices));
	calcAverageNormals(indices2, &vertices2, sizeof(indices2) / sizeof(*indices2));
	//calcAverageNormals(floorIndices, &floorVertices, sizeof(floorIndices) / sizeof(*floorIndices));

	Mesh* obj1 = new Mesh();
	obj1->CreateMeshIndex(&vertices, indices, sizeof(indices) / sizeof(*indices));
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMeshIndex(&vertices2, indices2, sizeof(indices2) / sizeof(*indices2));
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMeshIndex(&floorVertices, floorIndices, sizeof(floorIndices) / sizeof(*floorIndices));
	meshList.push_back(obj3);

	float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};


	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void CreateShaders()
{
	// Shaders
	geometryShader.CreateFromFiles("Shaders/geometry_shader.vert", "Shaders/geometry_shader.frag");
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	screenShader.CreateFromFiles("Shaders/anti_aliasing.vert", "Shaders/anti_aliasing.frag");
}

void ComputePositionOffsets(float& fXOffset, float& fZOffset)
{
	const float fLoopDuration = 20.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime = glfwGetTime();

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	//fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 10.0f;
	fZOffset = (sinf(fCurrTimeThroughLoop * fScale) * 10.0f);
}

void RenderScene(Shader* shader)
{
	if (direction)
	{
		triOffset += triIncrement * deltaTime;
	}
	else {
		triOffset -= triIncrement * deltaTime;
	}
	if (abs(triOffset) >= triMaxOffset) {
		direction = !direction;
	}
	curAngle += 30.0f * deltaTime;
	if (curAngle >= 360.0f) curAngle = 0.0f;

	brickNormal.UseTexture(GL_TEXTURE2);

	// CUBE
	shader->setBool("shouldUseTexture", 1);
	shader->setBool("shouldUseNormalMap", 1);
	brickDiffuse.UseTexture(GL_TEXTURE0);
	shinyMaterial.UseMaterial(shader);
	glm::mat4 model(1.0f); // Identity matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); // Apply a translation matrix to the model matrix
	//model = glm::rotate(model, glm::radians(curAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 1.0f));
	shader->setMat4("model", model);
	// Set the inverse transpose model in the CPU not GPU since it will have to do it per vertex otherwise
	shader->setMat4("inverseTransposeModel", glm::transpose(glm::inverse(model)));
	meshList[0]->RenderMeshIndex(); // Render object

	// SUN
	shader->setBool("shouldUseTexture", 0);
	shader->setBool("shouldUseNormalMap", 0);
	dullMaterial.UseMaterial(shader);
	model = glm::mat4(1.0f); // Identity matrix
	model = glm::translate(model, glm::vec3(xOffset, yOffset, zOffset));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	shader->setMat4("model", model);
	shader->setMat4("inverseTransposeModel", glm::transpose(glm::inverse(model)));
	meshList[1]->RenderMeshIndex();

	// FLOOR
	shader->setBool("shouldUseTexture", 1);
	shader->setBool("shouldUseNormalMap", 0);
	plainTexture.UseTexture(GL_TEXTURE0);
	shinyMaterial.UseMaterial(shader);
	model = glm::mat4(1.0f); // Identity matrix
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	shader->setMat4("model", model);
	shader->setMat4("inverseTransposeModel", glm::transpose(glm::inverse(model)));
	meshList[2]->RenderMeshIndex();

	//multiSampler.BlitFrameBuffer();
}

void DirectionalShadowMapPass(DirectionalLight* light)
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

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	geometryShader.UseShader(); 

	glViewport(0, 0, mainWindow.getBufferWidth(), mainWindow.getBufferHeight());

	geometryShader.setMat4("projection", projectionMatrix);
	geometryShader.setMat4("view", viewMatrix);
	geometryShader.setVec3("eyePosition", glm::vec3(camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z));
	geometryShader.setInt("theTexture", 0); //GL_TEXTURE0

	mainLight.GetShadowMap()->Read(GL_TEXTURE1);
	geometryShader.setInt("directionalShadowMap", 1);
	geometryShader.setInt("normalMap", 2);

	geometryShader.SetDirectionalLight(&mainLight);
	geometryShader.SetPointLights(pointLights, pointLightCount);

	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	geometryShader.setMat4("directionalLightTransform", lightTransform);

	RenderScene(&geometryShader);
}

int main()
{
	mainWindow = Window(1600, 900);
	mainWindow.Initialise();

	// Start Pos (x,y,z)
	// Start Up (x,y,z)
	// GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -10.0f, 5.0f, 0.05f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();

	brickDiffuse = Texture("Textures/brickwall.jpg");
	brickDiffuse.LoadTexture();
	brickNormal = Texture("Textures/brickwall_normal.jpg");
	brickNormal.LoadTexture();
	brickDisplacement = Texture("Textures/bricks2_disp.jpg");
	brickDisplacement.LoadTexture();


	// shader configuration
	// --------------------
	screenShader.UseShader();
	screenShader.setInt("screenTexture", 0);

	// framebuffer configuration
	// -------------------------
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mainWindow.getBufferWidth(), mainWindow.getBufferHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mainWindow.getBufferWidth(), mainWindow.getBufferHeight()); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	shinyMaterial = Material(1.0f, 128);
	dullMaterial = Material(0.3f, 4);

	CreateObjects();
	CreateShaders();

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferWidth(), 0.1f, 100.0f);


	// Red, Green, Blue, ambientIntensity, diffuseIntensity, Pos(XYZ), 
	mainLight = DirectionalLight(8192, 8192, 1.0f, 1.0f, 1.0f,
		0.1f, 0.7f,
		xOffset, yOffset, zOffset);
	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f,
		0.1f, 0.7f,
		-4.0f, 2.0f, -3.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		0.1f, 0.7f,
		4.0f, 2.0f, -3.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {
		// Delta time for synchronising events
		double now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Business logic for application
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// render
		// ------
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ComputePositionOffsets(xOffset, zOffset);
		// Red, Green, Blue, ambientIntensity, diffuseIntensity, Pos(XYZ), 
		mainLight.UpdatePosition(xOffset, yOffset, zOffset);

		//DirectionalShadowMapPass(&mainLight);
		RenderPass(projection, camera.calculateViewMatrix()); // Draw to GBuffer FrameBuffer

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.UseShader();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);		

		mainWindow.swapBuffers();
	}

	return 0;
}