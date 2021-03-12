#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace opengl {
	struct Vertex {
		glm::vec3 pos; // vertex position (x,y,z)
		glm::vec3 col; // vertex color (r,g,b)
		glm::vec2 tex; // Texture coords (u, v)
		glm::vec3 normal; // Normals
	};
}

