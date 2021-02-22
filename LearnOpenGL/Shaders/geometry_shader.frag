#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;
//in mat3 mat3 TBN;

out vec4 colour;

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

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform sampler2D normalMap;

uniform Material material;
uniform bool shouldUseTexture;
uniform bool shouldUseNormalMap;

uniform vec3 eyePosition;

float CalcDirectionalShadowFactor()
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5; // Scale between 0 and 1 from -1 and 1
	
	//float closest = texture(directionalShadowMap, projCoords.xy).r;
	float current = projCoords.z;
	
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(directionalLight.direction);
	
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);
	
	float shadow = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for (int i = -1; i <= 1; i++)
	{
		for (int k = -1; k <= 1; k++)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(i,k) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;;
		}
	}
	shadow /= 9.0;
	
	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
	}
	
	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;
	
	// Normal mapping
	vec3 normal = normalize(Normal);
	if (shouldUseNormalMap)
	{
		// obtain normal from normal map in range [0,1]
		normal = normalize(texture(normalMap, TexCoord).rgb*2.0 - 1.0);
	}
	vec3 lightDir = normalize(direction);
	
	float diffuseFactor = max(dot(normal, lightDir), 0.0f);
	vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	vec4 specularColour = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(lightDir, normal));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour + (1.0 - shadowFactor) * (diffuseColour + specularColour));
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor();
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPos - pointLights[i].position;
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 colour = CalcLightByDirection(pointLights[i].base, direction, 0.0f);
		float attenuation = pointLights[i].exponent * distance * distance +
							pointLights[i].linear * distance +
							pointLights[i].constant;
		
		totalColour += (colour / attenuation);
	}
	
	return totalColour;
}

void main()
{
	if (shouldUseTexture)
	{
		vec4 finalColour = CalcDirectionalLight();
		finalColour += CalcPointLights();
		
		colour = texture(theTexture, TexCoord) * finalColour;
	}
	else {
		colour = vCol;
	}
}