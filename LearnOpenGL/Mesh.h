#pragma once

#include <GLAD\glad.h>
#include <vector>
#include "Utilities.h"

class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat *vertices, unsigned int numOfVertices);
	void CreateMeshIndex(std::vector<Vertex>* vertices, unsigned int* indices, int numOfIndices);
	void RenderMesh();
	void RenderMeshIndex();
	void ClearMesh();
	~Mesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	GLsizei verticeCount;
};

