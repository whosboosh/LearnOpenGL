#include <stdio.h>
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, IBO, shader, uniformModel;
const float toRadians = 3.14159265f / 180.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float curAngle = 0.0f;

float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
bool sizeDirection = false;

// Vertex Shader
static const char* vShader = "						\n\
#version 330										\n\
uniform mat4 model;									\n\
out vec4 lerpColor;									\n\
layout(location = 0) in vec3 pos;					\n\
													\n\
void main()											\n\
{													\n\
	gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);\n\
	lerpColor = vec4(clamp(pos,0.0,1.0), 1.0f);		\n\
													\n\
}";

// Frag shader
static const char* fShader = "						\n\
#version 330										\n\
													\n\
out vec4 color;										\n\
in vec4 lerpColor;									\n\
													\n\
void main()											\n\
{													\n\
	color = lerpColor;								\n\
}";

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader program %s\n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader) {
		printf("Error creating shader program!");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };


	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program %s\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Failed to validate shader %s\n", eLog);
	}

}

void CreateTriangle() {
	GLfloat verticies[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	unsigned int indicies[] = {
		0,3,1,
		1,2,3,
		2,3,0,
		0,1,2
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
int main()
{
	// Initalise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use core mode (not immediate)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW initislisation failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Uniform
	uniformModel = glGetUniformLocation(shader, "model");

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Business logic for application
		glfwPollEvents();

		// Clear window
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);


		if (direction)
		{
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}
		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		curAngle += 0.1f;
		if(curAngle >= 360) {
			curAngle -= 360;
		}

		if (sizeDirection) {
			curSize += 0.001f;
		}
		else {
			curSize -= 0.001f;
		}
		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		glm::mat4 model(1.0f); // Identity matrix
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f)); // Apply a translation matrix to the model matrix
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}