#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 directionalLightTransform;
uniform mat4 model;

out vec2 TexCoords;
out vec4 DirectionalLightSpacePos;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
	
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(aPos, 1.0);
}