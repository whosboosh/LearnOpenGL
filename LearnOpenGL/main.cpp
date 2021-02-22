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

unsigned int pointLightCount = 0;

float xOffset = 0.0f;
float yOffset = -6.0f;
float zOffset = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;

Shader geometryShader;
Shader directionalShadowShader;

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

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices,
	unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	// For each face in a mesh, ln0, ln1, ln2 correspond to each vertex of that face.
	// We want to create two new vector 
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int ln0 = indices[i] * vLength;
		unsigned int ln1 = indices[i + 1] * vLength;
		unsigned int ln2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[ln1] - vertices[ln0], vertices[ln1 + 1] - vertices[ln0 + 1], vertices[ln1 + 2] - vertices[ln0 + 2]);
		glm::vec3 v2(vertices[ln2] - vertices[ln0], vertices[ln2 + 1] - vertices[ln0 + 1], vertices[ln2 + 2] - vertices[ln0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		ln0 += normalOffset; ln1 += normalOffset; ln2 += normalOffset;
		vertices[ln0] += normal.x; vertices[ln0 + 1] += normal.y; vertices[ln0 + 2] += normal.z;
		vertices[ln1] += normal.x; vertices[ln1 + 1] += normal.y; vertices[ln1 + 2] += normal.z;
		vertices[ln2] += normal.x; vertices[ln2 + 1] += normal.y; vertices[ln2 + 2] += normal.z;
	}
	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CalcNormals(GLfloat* vertices, unsigned int verticeCount, unsigned int normalOffset)
{
	for (int i = 0; i < verticeCount / 3; i++)
	{
		unsigned int ln0 = i * 24;
		unsigned int ln1 = i * 24 + 8;
		unsigned int ln2 = i * 24 + 16;

		glm::vec3 v1(vertices[ln1] - vertices[ln0], vertices[ln1 + 1] - vertices[ln0 + 1], vertices[ln1 + 2] - vertices[ln0 + 2]);
		glm::vec3 v2(vertices[ln2] - vertices[ln0], vertices[ln2 + 1] - vertices[ln0 + 1], vertices[ln2 + 2] - vertices[ln0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		ln0 += normalOffset; ln1 += normalOffset; ln2 += normalOffset;
		vertices[ln0] += normal.x; vertices[ln0 + 1] += normal.y; vertices[ln0 + 2] += normal.z;
		vertices[ln1] += normal.x; vertices[ln1 + 1] += normal.y; vertices[ln1 + 2] += normal.z;
		vertices[ln2] += normal.x; vertices[ln2 + 1] += normal.y; vertices[ln2 + 2] += normal.z;
	}
}

void CreateObjects() {
	GLfloat oldVertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Front left up
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Front left down
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Front right up
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Front right down

		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Back left Up
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Back left down
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Back right up
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f  // Back right down
	};


	GLfloat vertices[] = {
		// Frit
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, //0
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f,

		 // Right
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, //4
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f,

		 // Back
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //8
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		// Left
		-1.0f, -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, //12
		-1.0f, -1.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f,

		// Bottom
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //16
		 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		 // Top
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, //20 /LF
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, // RF
		-1.0f,  1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, // LB
		 1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // RB
	};
	unsigned int indices[] = {
		1,3,2, 2,0,1,  //Face front
		4,7,6, 6,5,4, //Face right
		10,8,9, 9,11,10, // Back
		14,12,13, 13,15,14, // Left
		16,17,19, 19,18,16, // Bottom
		23,22,20, 20,21,23, // Top
	};

	GLfloat vertices2[] = {
		//	x	   y      z     u     v   nx   ny   nz
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
	};

	unsigned int indices2[] = {
		0,3,1,
		1,2,3,
		2,3,0,
		0,1,2
	};

	int numIndices = sizeof(indices) / sizeof(*indices);
	//CalcNormals(vertices, 12, 5);
	calcAverageNormals(indices, numIndices, vertices, sizeof(vertices) / sizeof(*vertices), 8, 5);
	Mesh* obj1 = new Mesh();
	obj1->CreateMeshIndex(vertices, indices, sizeof(vertices) / sizeof(*vertices), numIndices);
	meshList.push_back(obj1);

	numIndices = sizeof(indices2) / sizeof(*indices2);
	calcAverageNormals(indices2, numIndices, vertices2, 32, 8, 5);
	Mesh* obj2 = new Mesh();
	obj2->CreateMeshIndex(vertices2, indices2, (sizeof(vertices2) / sizeof(*vertices2)), numIndices);
	meshList.push_back(obj2);

	std::cout << vertices[5] << " " << vertices[6] << " " << vertices[7] << "\n";
	std::cout << vertices[14] << " " << vertices[15] << " " << vertices[16] << "\n";
	std::cout << vertices[22] << " " << vertices[23]<< " " << vertices[24] << "\n";
	std::cout << vertices[30] << " " << vertices[31] << " " << vertices[32] << "\n";

	std::cout << "RIGHT FACE" << "\n";

	std::cout << vertices[38] << " " << vertices[39] << " " << vertices[40] << "\n";
	std::cout << vertices[46] << " " << vertices[47] << " " << vertices[48] << "\n";
	std::cout << vertices[54] << " " << vertices[55] << " " << vertices[56] << "\n";
	std::cout << vertices[62] << " " << vertices[63] << " " << vertices[64] << "\n";

	GLfloat floorVertices[] = {
		-20.0f, 0.0f, -20.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,//BL
		20.0f, 0.0f, -20.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,//BR
		-20.f, 0.0f, 20.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,//FL
		20.0f, 0.0f, 20.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f//FR
	};


	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	Mesh* obj3 = new Mesh();
	obj3->CreateMeshIndex(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	// Shaders
	geometryShader.CreateFromFiles("Shaders/geometry_shader.vert", "Shaders/geometry_shader.frag");
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
}

void ComputePositionOffsets(float& fXOffset, float& fZOffset)
{
	const float fLoopDuration = 20.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime = glfwGetTime();

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 10.0f;
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
	model = glm::translate(model, -glm::vec3(xOffset, yOffset, zOffset));
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

	shinyMaterial = Material(0.5, 128);
	dullMaterial = Material(0.3f, 4);

	CreateObjects();
	CreateShaders();

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferWidth(), 0.1f, 100.0f);


	// Red, Green, Blue, ambientIntensity, diffuseIntensity, Pos(XYZ), 
	mainLight = DirectionalLight(8192, 8192, 1.0f, 1.0f, 1.0f,
		0.1f, 0.8f,
		xOffset, yOffset, zOffset);
	pointLights[0] = PointLight(0.0f, 1.0f, 0.0f,
		0.1f, 0.7f,
		-2.0f, 2.0f, -3.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 0.0f, 1.0f,
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

		// Clear window
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ComputePositionOffsets(xOffset, zOffset);
		// Red, Green, Blue, ambientIntensity, diffuseIntensity, Pos(XYZ), 
		mainLight.UpdatePosition(xOffset, yOffset, zOffset);

		DirectionalShadowMapPass(&mainLight);
		RenderPass(projection, camera.calculateViewMatrix()); // Draw to GBuffer FrameBuffer

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}