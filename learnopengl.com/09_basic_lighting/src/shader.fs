
#version 330 core

out vec4 FragColour;

// in  vec2 TexCoord;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform vec3 objectColour;
uniform vec3 lightColour;

void main()
{
    // FragColour = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    const float ambientStrength = 0.1;
    vec3  ambient         = ambientStrength * lightColour;
    vec3  result          = ambient * objectColour;

    FragColour = vec4(result, 1.0);
}
