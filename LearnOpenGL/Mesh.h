#pragma once

#include <GLAD\glad.h>
#include <vector>
#include "Utilities.h"

class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat *vertices, unsigned int verticeCount, unsigned int numOfVertices);
	void CreateMeshIndex(std::vector<Vertex>* vertices, unsigned int* indices, int numOfIndices);
	void RenderMesh(unsigned int texture);
	void RenderMeshIndex();
	void ClearMesh();
	~Mesh();
private:
	unsigned int VAO, VBO, IBO;
	GLsizei indexCount;
	GLsizei verticeCount;
};

