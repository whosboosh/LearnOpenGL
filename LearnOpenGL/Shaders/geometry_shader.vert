#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 vCol;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 inverseTransposeModel;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	
	TexCoord = tex;
	
	Normal = mat3(inverseTransposeModel) * norm;
	
	FragPos = vec3(model * vec4(pos, 1.0)); 

	vCol = clamp(pos, 0.0f, 1.0f);
}