
#version 330 core

out vec4 FragColour;

in  vec3 FragPos;
in  vec3 Normal;
// in  vec2 TexCoord;

uniform vec3 lightPos;
// uniform sampler2D texture1;
// uniform sampler2D texture2;

uniform vec3 objectColour;
uniform vec3 lightColour;

void main()
{
    // FragColour = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    const float ambientStrength = 0.1;
    vec3  ambient         = ambientStrength * lightColour;

    vec3  norm            = normalize(Normal);
    vec3  lightDir        = normalize(lightPos - FragPos);
    float diff            = max(dot(norm, lightDir), 0.0);
    vec3  diffuse         = diff * lightColour;

    vec3  result          = (ambient + diffuse) * objectColour;

    FragColour = vec4(result, 1.0);
}
