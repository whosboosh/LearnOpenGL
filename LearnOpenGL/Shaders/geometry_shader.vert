#version 330

layout (location = 0) in vec3 pos;
layout(location = 1) in vec3 col;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 norm;
out vec3 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 inverseTransposeModel;
uniform mat4 directionalLightTransform; 

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = col;
	
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);
	
	TexCoord = tex;
	
	Normal = mat3(inverseTransposeModel) * norm;
	
	FragPos = vec3(model * vec4(pos, 1.0)); 

	
}