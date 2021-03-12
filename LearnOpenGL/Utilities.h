#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

struct Vertex {
	glm::vec3 pos; // vertex position (x,y,z)
	glm::vec3 col; // vertex color (r,g,b)
	glm::vec2 tex; // Texture coords (u, v)
	glm::vec3 normal; // Normals
};

namespace opengl {
	static void calcAverageNormals(std::vector<Vertex>* vertices, std::vector<uint32_t>* indices)
	{
		// For each face in a mesh, ln0, ln1, ln2 correspond to each vertex of that face.
		// We want to create two new vector

		for (size_t i = 0; i < indices->size(); i += 3)
		{
			unsigned int ln0 = indices->at(i);
			unsigned int ln1 = indices->at(i + 1);
			unsigned int ln2 = indices->at(i + 2);

			glm::vec3 v1(vertices->at(ln1).pos.x - vertices->at(ln0).pos.x, vertices->at(ln1).pos.y - vertices->at(ln0).pos.y, vertices->at(ln1).pos.z - vertices->at(ln0).pos.z);
			glm::vec3 v2(vertices->at(ln2).pos.x - vertices->at(ln0).pos.x, vertices->at(ln2).pos.y - vertices->at(ln0).pos.y, vertices->at(ln2).pos.z - vertices->at(ln0).pos.z);

			glm::vec3 normal = glm::cross(v1, v2);
			normal = glm::normalize(normal);

			vertices->at(ln0).normal.x += normal.x; vertices->at(ln0).normal.y += normal.y; vertices->at(ln0).normal.z += normal.z;
			vertices->at(ln1).normal.x += normal.x; vertices->at(ln1).normal.y += normal.y; vertices->at(ln1).normal.z += normal.z;
			vertices->at(ln2).normal.x += normal.x; vertices->at(ln2).normal.y += normal.y; vertices->at(ln2).normal.z += normal.z;
		}

		for (size_t i = 0; i < vertices->size(); i++)
		{
			glm::vec3 vec(vertices->at(i).normal.x, vertices->at(i).normal.y, vertices->at(i).normal.z);
			vec = -glm::normalize(vec); // Inverse normal because maths xd
			vertices->at(i).normal.x = vec.x;
			vertices->at(i).normal.y = vec.y;
			vertices->at(i).normal.z = vec.z;
		}
	}

	static void inverseNormal(std::vector<Vertex>* vertices)
	{
		for (int i = 0; i < vertices->size(); i++)
		{
			glm::vec3 normal = vertices->at(i).normal;
			vertices->at(i).normal = -normal;
		}
	}
}