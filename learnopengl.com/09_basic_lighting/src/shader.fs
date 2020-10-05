
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
uniform vec3 viewPos;

void main()
{
    // FragColour = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

    const float ambientStrength  = 0.1;
    vec3  ambient  = ambientStrength * lightColour;

    vec3  norm     = normalize(Normal);
    vec3  lightDir = normalize(lightPos - FragPos);
    float diff     = max(dot(norm, lightDir), 0.0);
    vec3  diffuse  = diff * lightColour;

    const float specularStrength = 0.5;
    vec3  viewDir    = normalize(viewPos - FragPos);
    vec3  reflectDir = reflect(-lightDir, norm);
    float spec       = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3  specular   = specularStrength * spec * lightColour;

    vec3  result  = (ambient + diffuse + specular) * objectColour;

    FragColour = vec4(result, 1.0);
}
