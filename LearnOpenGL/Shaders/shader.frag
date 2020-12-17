#version 330

in vec4 vCol;
in vec2 texCoord;

out vec4 colour;

struct DirectionalLight {
	 vec3 colour;
	 float ambientIntensity;
};

uniform DirectionalLight directionalLight;
uniform sampler2D theTexture;

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	colour = texture(theTexture, texCoord) * ambientColour;
}