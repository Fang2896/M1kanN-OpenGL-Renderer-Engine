
#version 410 core

struct DirectLight {
    vec3 direction;
    vec3 ambientColor;     // Light color
    vec3 diffuseColor;     // Light color
    vec3 specularColor;     // Light color
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float falloff;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float falloff;
};

struct ModelMaterial {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
};

uniform vec3 viewPos;
uniform DirectLight directLight;
uniform ModelMaterial material;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


void main()
{
    // Ambient
    vec3 ambientColor = 0.3f * directLight.ambientColor * directLight.intensity; // 常数环境光颜色

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(directLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuseColor =
        (texture(material.texture_diffuse1, TexCoords).rgb
        + texture(material.texture_diffuse2, TexCoords).rgb
        + texture(material.texture_diffuse3, TexCoords).rgb) * diff * directLight.intensity;

    // Specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // 注意这个负号
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specularColor =
        (texture(material.texture_specular1, TexCoords).rgb
        + texture(material.texture_specular2, TexCoords).rgb) * specularStrength * spec * directLight.intensity;

    vec3 result = ambientColor + directLight.diffuseColor * diffuseColor + directLight.specularColor * specularColor;
    // vec3 result = ambientColor + directLight.color * (diffuseColor + specularColor);
    FragColor = vec4(result, 1.0);

    // FragColor = texture(material.texture_diffuse1, TexCoords);
}
