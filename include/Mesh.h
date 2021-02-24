#include <iostream>
#include "./GLAD/glad.h"

class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat *vertices, unsigned int numOfVertices);
	void CreateMeshIndex(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void RenderMeshIndex();
	void ClearMesh();
	~Mesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

