#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;



out vec4 colour;


uniform sampler2D theTexture;



uniform bool shouldUseTexture;

void main()
{
	if (shouldUseTexture)
	{
		//vec4 finalColour = CalcDirectionalLight();
		//finalColour += CalcPointLights();
		
colour = vCol;
	}
	else {
		colour = vCol;
	}
}