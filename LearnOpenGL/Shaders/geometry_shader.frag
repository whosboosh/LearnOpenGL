#version 330

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 vCol;

uniform sampler2D theTexture;
uniform bool shouldUseTexture;

void main()
{
	// store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color
	if (shouldUseTexture)
	{
	    gAlbedoSpec.rgb = texture(theTexture, TexCoord).rgb;
	} else {
		gAlbedoSpec.rgb = vCol;
	}
}