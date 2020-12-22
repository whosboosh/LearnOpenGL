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
#include "Light.h"
#include "Material.h"

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture woodTexture;

Material shinyMaterial;
Material dullMaterial;

Light mainLight;

const float toRadians = 3.14159265f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 2.0f;
float triIncrement = 0.5f;

float curAngle = 0.0f;

float deltaTime = 0.0f;
float lastTime = 0.0f;

float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
bool sizeDirection = false;

// Shaders
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat *vertices,
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
		vertices[ln0] += normal.x; vertices[ln0  + 1] += normal.y; vertices[ln0 + 2] += normal.z;
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
	for (int i = 0; i < verticeCount/3; i++)
	{
		unsigned int ln0 = i*24;
		unsigned int ln1 = i*24 + 8;
		unsigned int ln2 = i*24 + 16;

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
	GLfloat verticesIndex[] = {
	//	x	   y      z     u     v     nx    ny    nz
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,

		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	unsigned int indices[] = {
		0,3,1,
		1,2,3,
		2,3,0,
		0,1,2
	};

	calcAverageNormals(indices, 12, verticesIndex, 32, 8, 5);
	CalcNormals(vertices, 12, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(verticesIndex, indices, (sizeof(verticesIndex) / sizeof(*verticesIndex)), 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(verticesIndex, indices, (sizeof(verticesIndex) / sizeof(*verticesIndex)), 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();

	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void ComputePositionOffsets(float& fXOffset, float& fYOffset)
{
	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;

	float fElapsedTime = glfwGetTime();

	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

	fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 6.0f;
	fYOffset = (sinf(fCurrTimeThroughLoop * fScale) * 6.0f) + -3.0f;
}

int main()
{
	mainWindow = Window(1600, 900);
	mainWindow.Initialise();

	// Start Pos (x,y,z)
	// Start Up (x,y,z)
	// GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -10.0f, 5.0f, 0.05f);

	woodTexture = Texture("Textures/brick.png");
	woodTexture.LoadTexture();

	shinyMaterial = Material(0.6f, 32);
	dullMaterial = Material(0.3f, 4);

	// Red, Green, Blue, ambientIntensity, Pos(XYZ), diffuseIntensity
	mainLight = Light(1.0f, 1.0f, 1.0f, 0.1f, 3.0f, 1.5f, -1.0f, 0.4f);

	CreateObjects();
	CreateShaders();

	// Uniform
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
		uniformAmbientIntensity = 0, uniformAmbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0,
		uniformSpecularIntensity = 0, uniformSpecularShininess = 0, uniformEyePosition = 0;

	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
	uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
	uniformDirection = shaderList[0].GetDirectionLocation();
	uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformSpecularShininess = shaderList[0].GetSpecularShininessLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferWidth(), 0.1f, 100.0f);

	float yOffset = 0.0f;
	float xOffset = 0.0f;

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {
		// Delta time for synchronising events
		float now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Business logic for application
		glfwPollEvents();

		// Clear window
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		shaderList[0].UseShader();

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

		ComputePositionOffsets(xOffset, yOffset);
		// Red, Green, Blue, ambientIntensity, Pos(XYZ), diffuseIntensity
		mainLight = Light(1.0f, 1.0f, 1.0f, 0.1f, xOffset, 2.0f, yOffset, 0.6f);

		// Use light source
		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour , uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0f); // Identity matrix
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -2.5f)); // Apply a translation matrix to the model matrix
		//model = glm::rotate(model, glm::radians(curAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// Textures
		woodTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f); // Identity matrix
		model = glm::translate(model, glm::vec3(xOffset, 2.0f, yOffset));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformSpecularShininess);
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}