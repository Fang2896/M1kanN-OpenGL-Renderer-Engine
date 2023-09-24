
#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float falloff;
};

uniform vec3 shapeColor;
uniform vec3 viewPos;

uniform PointLight pointLight;

void main()
{
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * pointLight.color;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pointLight.color;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * pointLight.color;

    vec3 result = (ambient + diffuse + specular) * shapeColor;
    FragColor = vec4(result, 1.0);
}
