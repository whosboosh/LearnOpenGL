#version 330
out vec4 outColour;

in vec2 TexCoords;
in vec4 DirectionalLightSpacePos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform sampler2D directionalShadowMap;
uniform sampler2D normalMap;

const int MAX_POINT_LIGHTS = 3;

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float shininess;
};
uniform int pointLightCount;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform Material material;